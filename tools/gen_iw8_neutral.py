"""Generate a neutral asset family from the IW8 structs and stream traversals.

Usage: python tools/gen_iw8_neutral.py <family spec .json> [--plan] [--check]

A family spec names the root structs of one or more asset types. The generator follows the structs that the roots
reach and takes what each pointer is from the stream traversals of src/games/iw8/zone/asset_streams.cpp: a string, an
asset reference (by name), or an array with a count member or a fixed count. It emits
  src/core/assets/<family>_data.hpp          the neutral types (named fields, vectors, strings, enum names)
  src/games/iw8/neutral/<family>.hpp/.cpp   to_neutral, from_neutral and native_size for each root.
--plan prints what the generator found and writes nothing, so the family can be reviewed first.

Nothing is dropped silently: a pointer without a stream rule, a padding member, an enum value without a name and the
bytes of a character buffer after its terminator make to_neutral fail unless they are zero, and the adapter then
declines the asset (it stays native)."""
import collections
import json
import os
import re
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.normpath(os.path.join(HERE, '..'))
STRUCTS = os.path.join(ROOT, 'src', 'games', 'iw8', 'structs.hpp')
STREAMS = os.path.join(ROOT, 'src', 'games', 'iw8', 'zone', 'asset_streams.cpp')

SCALARS = {'std::uint8_t', 'std::uint16_t', 'std::uint32_t', 'std::uint64_t', 'std::int8_t', 'std::int16_t', 'std::int32_t',
           'std::int64_t', 'float', 'bool', 'char'}
ALIASES = {'int': 'std::int32_t', 'unsigned int': 'std::uint32_t', 'std::size_t': 'std::uint64_t'}
# Unions of a float vector and its named parts are plain vectors of floats.
VECTORS = {'vec2_t': 2, 'vec3_t': 3, 'vec4_t': 4, 'float4': 4}
PADDING = re.compile(r'^(m_)?pad(ding)?(_?\d+)?$|^unused\w*$|^reserved\d*$')


def tokens(name):
    name = re.sub(r'([a-z0-9])([A-Z])', r'\1 \2', name)
    name = re.sub(r'([A-Z]+)([A-Z][a-z])', r'\1 \2', name)
    return [t for t in re.split(r'[ _]+', name) if t]


class Source:
    """The structs, unions and enums of structs.hpp."""

    def __init__(self):
        text = open(STRUCTS, encoding='utf-8').read()
        self.structs = {}
        # A body is the lines with two tabs: an empty body must not run on into the next declaration.
        for kind, name, body in re.findall(r'\n\t(struct|union) (\w+)\n\t\{\n((?:\t\t[^\n]*\n)*)\t\};', text):
            self.structs[name] = (kind, body)
        self.enums = {}
        for name, base, body in re.findall(r'\n\tenum (\w+) : ([\w:]+)\n\t\{\n((?:\t\t[^\n]*\n)*)\t\};', text):
            values = collections.OrderedDict()
            for member, value in re.findall(r'^\t\t(\w+) = (-?\d+),?$', body, re.M):
                values.setdefault(int(value), member)
            self.enums[name] = (base, values)

    @staticmethod
    def members(body):
        out = []
        for line in body.split('\n'):
            line = line.strip().replace('volatile ', '')
            if not line:
                continue
            m = re.match(r'^((?:const )?(?:unsigned int|[\w:]+))(\*+)?\s*(\w+)((?:\[\w+\])*)(?:\s*:\s*(\d+))?;$', line)
            if not m:
                raise SystemExit('cannot parse member: ' + line)
            type_name, stars, name, dims, bits = m.groups()
            type_name = type_name.replace('const ', '')
            out.append({'type': ALIASES.get(type_name, type_name), 'pointer': len(stars or ''), 'name': name,
                        'dims': [int(d) for d in re.findall(r'\[(\d+)\]', dims)], 'bits': int(bits) if bits else None})
        return out


class Rules:
    """What each pointer member is, from the stream traversals."""

    def __init__(self, source):
        self.source = source
        text = open(STREAMS, encoding='utf-8').read()
        self.strings = set()
        self.assets = {}
        self.arrays = {}
        self.element_sizes = {}
        member = r'(?:\w+(?:\[\w+\])?\.)*?(\w+)'
        for m in re.finditer(r'stream\.string\(' + member + r'\);', text):
            self.strings.add(m.group(1))
        for m in re.finditer(r'stream\.asset\((ASSET_TYPE_\w+), ' + member + r'\);', text):
            self.assets[m.group(2)] = m.group(1)
        for call in re.finditer(r'stream\.(?:inline_array|array|elements)\(', text):
            arguments = self.arguments(text, call.end())
            if len(arguments) < 3:
                continue
            owner = re.search(r'(\w+)(?:\[[^\]]*\])?$', arguments[0])
            # The first argument is a member of some object: a bare name is the function's own array.
            if not owner or '.' not in arguments[0] or not re.match(r'^[\w]+(?:\[[^\]]*\])?(?:\.\w+(?:\[[^\]]*\])?)*$', arguments[0]):
                continue
            field, count = owner.group(1), arguments[-1].strip()
            if re.fullmatch(r'\d+', count):
                rule = ('literal', int(count))
            elif re.fullmatch(r'(?:\w+(?:\[\w+\])?\.)*(\w+)', count):
                rule = ('member', re.fullmatch(r'(?:\w+(?:\[\w+\])?\.)*(\w+)', count).group(1))
            elif re.fullmatch(r'\*reinterpret_cast<const ([\w:]+)\*>\(&\w+(?:\[\w+\])?\)', count):
                rule = ('first', re.fullmatch(r'\*reinterpret_cast<const ([\w:]+)\*>\(&\w+(?:\[\w+\])?\)', count).group(1))
            elif 'outer_' not in count and 'reinterpret_cast' not in count and re.search(r'\.\w+', count):
                rule = ('expr', count)
            else:
                continue
            if rule not in self.arrays.setdefault(field, []):
                self.arrays[field].append(rule)
            if call.group(0).startswith('stream.elements') and re.fullmatch(r'\d+', arguments[2]):
                self.element_sizes.setdefault(field, set()).add(int(arguments[2]))

    @staticmethod
    def arguments(text, position):
        """The top-level arguments of the call whose opening parenthesis ends at position."""
        depth, angle, current, out = 1, 0, '', []
        while depth and position < len(text):
            char = text[position]
            position += 1
            if char in '([{':
                depth += 1
            elif char in ')]}':
                depth -= 1
                if not depth:
                    break
            if char == ',' and depth == 1:
                out.append(current)
                current = ''
            else:
                current += char
        out.append(current)
        return [a.strip() for a in out]

    @staticmethod
    def plain_scalar(m):
        """A member whose value can stand in a count expression: a number, not a pointer, array or named enum."""
        return not m['pointer'] and not m['dims'] and m['type'] in SCALARS

    def leading_scalar(self, members):
        """The members that lead to the scalar at offset 0 of a struct: the first member, or through nested structs."""
        path = []
        while members:
            first = members[0]
            path.append(first)
            if self.plain_scalar(first):
                return path
            if first['pointer'] or first['dims'] or first['type'] not in self.source.structs or \
                    self.source.structs[first['type']][0] != 'struct':
                return None
            members = Source.members(self.source.structs[first['type']][1])
        return None

    def array_count(self, field, members):
        """The count rule of an array member: the first candidate that the owner can satisfy."""
        names = {m['name'] for m in members}
        for rule in self.arrays.get(field, []):
            if rule[0] == 'literal':
                return rule
            if rule[0] == 'member' and rule[1] in names:
                return rule
            if rule[0] == 'first':
                path = self.leading_scalar(members)
                if path and path[-1]['type'].split('::')[-1] == rule[1].split('::')[-1]:
                    return ('expr', 'data.' + '.'.join(p['name'] for p in path))
            if rule[0] == 'expr':
                used = re.findall(r'\b\w+(?:\[\w+\])?\.(\w+)(?:\.\w+)*', rule[1])
                if all(n in names for n in used) and all(self.plain_scalar(m) for m in members if m['name'] in used):
                    return rule
        return None


class Family:
    def __init__(self, spec):
        self.spec = spec
        self.source = Source()
        self.rules = Rules(self.source)
        self.token_names = spec.get('tokens', {})
        self.struct_names = spec.get('structs', {})
        self.field_names = spec.get('fields', {})
        self.structs = collections.OrderedDict()
        self.enums = collections.OrderedDict()
        self.overlay_names = {}
        self.spec_flag_enums = set(spec.get('flag_enums', []))
        self.groups = spec.get('groups', {})
        self.problems = []
        for asset in spec['assets']:
            self.visit(asset['struct'])

    # ---- names -----------------------------------------------------------------------------------
    def words(self, name):
        return '_'.join(self.token_names.get(t.lower(), t.lower()) for t in tokens(name))

    def struct_name(self, name):
        for asset in self.spec['assets']:
            if asset['struct'] == name:
                return asset['neutral']
        return self.struct_names.get(name) or self.words(name)

    def field_name(self, owner, name):
        return self.field_names.get(owner + '.' + name) or self.words(name)

    # ---- classification --------------------------------------------------------------------------
    def classify(self, owner, m, members):
        """The kind of a member: how to_neutral reads it and from_neutral writes it."""
        name, type_name = m['name'], m['type']
        if PADDING.match(name):
            return {'kind': 'padding'}
        if type_name == 'scr_string_t' and not m['pointer']:
            if m['dims']:
                self.problems.append('%s.%s: an inline array of script strings' % (owner, name))
                return {'kind': 'unknown'}
            return {'kind': 'script_string'}
        if m['pointer'] == 2 and type_name == 'char':
            count = self.rules.array_count(name, members)
            if count:
                return {'kind': 'array', 'element': 'char*', 'native_element': 'const char*', 'count': count}
            return {'kind': 'padding'}
        if m['pointer'] == 1 and type_name == 'char':
            if name in self.rules.strings:
                return {'kind': 'string'}
            count = self.rules.array_count(name, members)
            if count:
                return {'kind': 'array', 'element': 'char', 'native_element': 'char', 'count': count}
            return {'kind': 'padding'}
        if m['pointer']:
            count = self.rules.array_count(name, members)
            if name in self.rules.assets:
                return {'kind': 'asset', 'asset': self.rules.assets[name], 'target': type_name}
            if count and m['pointer'] == 1:
                if not self.mappable(type_name):
                    self.problems.append('%s.%s: an array of %s has a stream rule and no mapping' % (owner, name, type_name))
                    return {'kind': 'unknown'}
                result = {'kind': 'array', 'element': type_name, 'native_element': type_name, 'count': count}
                declared = self.declared_size(type_name)
                for streamed in self.rules.element_sizes.get(name, ()):
                    if declared and streamed != declared:
                        group = self.groups.get(name)
                        if group and group * declared == streamed:
                            result['group'] = group
                        else:
                            self.problems.append('%s.%s: the stream element is %d bytes and %s is %d (add a group to the spec)' % (
                                owner, name, streamed, type_name, declared))
                            return {'kind': 'unknown'}
                return result
            return {'kind': 'padding'}
        if type_name == 'char' and len(m['dims']) == 1:
            return {'kind': 'text', 'size': m['dims'][0]}
        if type_name in self.source.enums and self.source.enums[type_name][1]:
            if type_name in self.spec_flag_enums:
                return {'kind': 'flags', 'enum': type_name}
            return {'kind': 'enum', 'enum': type_name}
        if type_name in self.source.enums or type_name in SCALARS:
            return {'kind': 'scalar'}
        vector = self.vector_of(type_name)
        if vector:
            return {'kind': 'vector', 'scalar': vector[0], 'size': vector[1]}
        if type_name in self.source.structs and self.source.structs[type_name][0] == 'union':
            return self.classify_overlay(owner, m)
        if type_name in self.source.structs:
            return {'kind': 'struct'}
        self.problems.append('%s.%s: unknown type %s' % (owner, name, type_name))
        return {'kind': 'unknown'}

    def declared_size(self, type_name):
        """The size in bytes of a scalar or vector type, or None when it is not known here."""
        if type_name in self.SIZES:
            return self.SIZES[type_name]
        vector = self.vector_of(type_name)
        if vector:
            return self.SIZES[vector[0]] * vector[1] if vector[0] in self.SIZES else None
        if type_name == 'float4':
            return 16
        if type_name in self.source.enums:
            return self.SIZES.get(ALIASES.get(self.source.enums[type_name][0], self.source.enums[type_name][0]))
        return None

    def mappable(self, type_name):
        """True when an array element of this type has a neutral form."""
        return (type_name in SCALARS or type_name in self.source.enums or type_name == 'scr_string_t' or type_name in VECTORS or
                (type_name in self.source.structs and self.source.structs[type_name][0] == 'struct') or bool(self.vector_of(type_name)))

    def vector_of(self, type_name):
        """(scalar type, count) of a vector type: a register type or a union that starts with a scalar array named v."""
        if type_name in VECTORS:
            return 'float', VECTORS[type_name]
        if type_name in self.source.structs and self.source.structs[type_name][0] == 'union':
            first = Source.members(self.source.structs[type_name][1])[0]
            if first['name'] == 'v' and len(first['dims']) == 1 and not first['pointer'] and first['type'] in SCALARS:
                return first['type'], first['dims'][0]
        return None

    SIZES = {'std::uint8_t': 1, 'std::int8_t': 1, 'bool': 1, 'char': 1, 'std::uint16_t': 2, 'std::int16_t': 2, 'std::uint32_t': 4,
             'std::int32_t': 4, 'float': 4, 'std::uint64_t': 8, 'std::int64_t': 8}

    def classify_overlay(self, owner, m):
        """A union of scalars of one size is kept as its bits under the names of its alternatives: which one applies is the
        owner's to say (a flag), and the data is the same in each."""
        alternatives = Source.members(self.source.structs[m['type']][1])
        sizes = set()
        for a in alternatives:
            type_name = a['type']
            if type_name in self.source.enums:
                type_name = ALIASES.get(self.source.enums[type_name][0], self.source.enums[type_name][0])
            if a['pointer'] or a['dims'] or type_name not in self.SIZES:
                self.problems.append('%s.%s: a union of %s is not mapped' % (owner, m['name'], m['type']))
                return {'kind': 'unknown'}
            sizes.add(self.SIZES[type_name])
        if len(sizes) != 1 or m['dims']:
            self.problems.append('%s.%s: a union of %s mixes sizes or is an array' % (owner, m['name'], m['type']))
            return {'kind': 'unknown'}
        return {'kind': 'overlay', 'bits': sizes.pop() * 8, 'alternatives': [a['name'] for a in alternatives]}

    def visit(self, name):
        if name in self.structs or self.vector_of(name) or name not in self.source.structs:
            return
        kind, body = self.source.structs[name]
        if kind == 'union':
            self.problems.append('%s: a union is not mapped' % name)
            return
        members = Source.members(body)
        classes = [self.classify(name, m, members) for m in members]
        self.structs[name] = list(zip(members, classes))
        for m, c in self.structs[name]:
            if c['kind'] == 'overlay':
                self.overlay_names[(name, m['name'])] = '_or_'.join(self.words(a) for a in c['alternatives'])
        # Counts that an array uses are derived from its length.
        counted = {}
        for m, c in self.structs[name]:
            if c['kind'] == 'array' and c['count'][0] == 'member':
                counted.setdefault(c['count'][1], []).append(m['name'])
        for m, c in self.structs[name]:
            if m['name'] in counted:
                c['counts'] = counted[m['name']]
        for m, c in self.structs[name]:
            if c['kind'] == 'array' and c['count'][0] == 'member' and not any(
                    n['name'] == c['count'][1] for n, _ in self.structs[name]):
                self.problems.append('%s.%s: its count %s is not in the struct' % (name, m['name'], c['count'][1]))
            if c['kind'] in ('struct',):
                self.visit(m['type'])
            if c['kind'] == 'array' and m['type'] in self.source.structs:
                self.visit(m['type'])
            if c['kind'] in ('enum', 'flags'):
                self.enums[m['type']] = self.source.enums[m['type']]

    # ---- types -----------------------------------------------------------------------------------
    KEYWORDS = {'class', 'default', 'delete', 'new', 'operator', 'register', 'template', 'union', 'this', 'not', 'and', 'or',
                'xor', 'private', 'public', 'namespace', 'typename', 'bool', 'signed', 'unsigned', 'int', 'long', 'short'}

    def member_name(self, owner, m):
        name = self.field_name(owner, m['name'])
        overlay = self.overlay_names.get((owner, m['name']))
        if overlay and m['name'].startswith('_') and (owner + '.' + m['name']) not in self.field_names:
            name = overlay
        return name + '_' if name in self.KEYWORDS else name

    def scalar_type(self, type_name):
        if type_name in self.source.enums:
            return ALIASES.get(self.source.enums[type_name][0], self.source.enums[type_name][0])
        return type_name

    # The namespace prefix of neutral types: empty in the data header, asset_data:: in the adapter.
    qual = ''

    def element_type(self, type_name):
        """The neutral type of an array element or an inline value of a type."""
        if type_name in ('scr_string_t', 'char*'):
            return 'std::optional<std::string>'
        vector = self.vector_of(type_name)
        if vector:
            return 'std::array<%s, %d>' % vector
        if type_name in self.source.structs:
            return self.qual + self.struct_name(type_name)
        return self.scalar_type(type_name)

    @staticmethod
    def nest(type_name, dims):
        for dim in reversed(dims):
            type_name = 'std::array<%s, %d>' % (type_name, dim)
        return type_name

    def neutral_type(self, m, c):
        kind = c['kind']
        if kind == 'scalar':
            return self.nest(self.scalar_type(m['type']), m['dims'])
        if kind == 'text':
            return 'std::string'
        if kind in ('string', 'asset', 'script_string'):
            return 'std::optional<std::string>'
        if kind == 'enum':
            return 'std::string'
        if kind == 'flags':
            return 'std::vector<std::string>'
        if kind == 'vector':
            return self.nest('std::array<%s, %d>' % (c['scalar'], c['size']), m['dims'])
        if kind == 'overlay':
            return 'std::uint%d_t' % c['bits']
        if kind == 'struct':
            return self.nest(self.qual + self.struct_name(m['type']), m['dims'])
        if kind == 'array':
            element = self.element_type(c['element'])
            if c.get('group'):
                element = 'std::array<%s, %d>' % (element, c['group'])
            if c['count'] == ('literal', 1):
                return 'std::optional<%s>' % element
            if c['count'][0] == 'literal':
                return 'std::optional<std::array<%s, %d>>' % (element, c['count'][1])
            return 'std::optional<std::vector<%s>>' % element
        raise SystemExit('no neutral type for %s' % kind)

    def emitted(self, members):
        """The members that become neutral fields: not padding and not a count that an array derives."""
        return [(m, c) for m, c in members if c['kind'] not in ('padding', 'unknown') and 'counts' not in c]

    def enum_table(self, type_name):
        base, values = self.source.enums[type_name]
        names = [tokens(v) for v in values.values()]
        common = 0
        while all(len(n) > common + 1 and n[common].lower() == names[0][common].lower() for n in names):
            common += 1
        return self.words(type_name) + '_names', [(k, '_'.join(t.lower() for t in n[common:])) for k, n in zip(values, names)]

    def order(self):
        """The structs, a struct after the structs it contains."""
        done, out = set(), []

        def visit(name):
            if name in done:
                return
            done.add(name)
            for m, c in self.structs[name]:
                if c['kind'] in ('struct', 'array') and m['type'] in self.structs:
                    visit(m['type'])
            out.append(name)
        for name in self.structs:
            visit(name)
        return out

    # ---- the data header -------------------------------------------------------------------------
    def header(self):
        family = self.spec['family']
        out = ['#pragma once', '', '#include "asset_data.hpp"', '#include <algorithm>', '#include <array>', '#include <cstdint>', '#include <optional>',
               '#include <stdexcept>', '#include <string>', '#include <string_view>', '#include <utility>', '#include <vector>', '',
               '// Neutral %s assets (docs/NEUTRAL_ASSETS.md), emitted by tools/gen_iw8_neutral.py from the IW8 structs and stream' % family,
               '// traversals: do not edit; change the spec (tools/neutral_families/%s.json) or the generator.' % family,
               'namespace zonetool::asset_data', '{']
        for type_name in self.enums:
            table, names = self.enum_table(type_name)
            out.append('inline constexpr std::array<std::pair<std::int64_t, std::string_view>, %d> %s{{' % (len(names), table))
            out.extend('\t{%d, "%s"},' % (value, name) for value, name in names)
            out.append('}};')
            out.append('')
        for name in self.order():
            out.extend(self.struct_decl(name))
        out.extend(self.validators())
        out.append('} // namespace zonetool::asset_data')
        return '\n'.join(out) + '\n'

    def struct_decl(self, name):
        lines = ['struct %s' % self.struct_name(name), '{']
        for m, c in self.emitted(self.structs[name]):
            note = ''
            if c['kind'] == 'overlay':
                note = ' // the bits of %s' % ' or '.join(c['alternatives'])
            lines.append('\t%s %s{};%s' % (self.neutral_type(m, c), self.member_name(name, m), note))
        lines += ['', '\tbool operator==(const %s&) const = default;' % self.struct_name(name), '};', '']
        return lines

    def count_groups(self, name):
        """The arrays of a struct by the count member they share."""
        groups = collections.OrderedDict()
        for m, c in self.structs[name]:
            if c['kind'] == 'array' and c['count'][0] == 'member':
                groups.setdefault(c['count'][1], []).append(m)
        return groups

    def validators(self):
        lines = []
        for name in self.order():
            neutral = self.struct_name(name)
            body = []
            for m, c in self.emitted(self.structs[name]):
                field = 'data.' + self.member_name(name, m)
                what = '%s.%s' % (neutral, self.member_name(name, m))
                if c['kind'] == 'text':
                    body.append('\tif (%s.size() >= %d)\n\t{\n\t\tthrow std::runtime_error("%s does not fit its %d character buffer");\n\t}\n'
                                % (field, c['size'], what, c['size']))
                elif c['kind'] == 'flags':
                    table = self.enum_table(m['type'])[0]
                    body.append('\tfor (const auto& name : %s)\n\t{\n\t\tif (std::ranges::none_of(%s, [&](const auto& entry) { return entry.second == name; }))\n'
                                '\t\t{\n\t\t\tthrow std::runtime_error("%s names no flag");\n\t\t}\n\t}\n' % (field, table, what))
                elif c['kind'] == 'enum':
                    table = self.enum_table(m['type'])[0]
                    body.append('\tif (std::ranges::none_of(%s, [&](const auto& entry) { return entry.second == %s; }))\n\t{\n'
                                '\t\tthrow std::runtime_error("%s names no value");\n\t}\n' % (table, field, what))
                elif c['kind'] == 'struct' and m['type'] in self.structs:
                    if m['dims']:
                        body.append('\tfor (const auto& item : %s)\n\t{\n\t\tvalidate(item);\n\t}\n' % field)
                    else:
                        body.append('\tvalidate(%s);\n' % field)
                elif False:
                    pass
                elif c['kind'] == 'array' and m['type'] in self.structs and c['count'] == ('literal', 1):
                    body.append('\tif (%s)\n\t{\n\t\tvalidate(*%s);\n\t}\n' % (field, field))
                elif c['kind'] == 'array' and m['type'] in self.structs:
                    body.append('\tif (%s)\n\t{\n\t\tfor (const auto& item : *%s)\n\t\t{\n\t\t\tvalidate(item);\n\t\t}\n\t}\n' % (field, field))
            for m, c in self.emitted(self.structs[name]):
                if c['kind'] == 'array' and c['count'][0] == 'expr':
                    field = 'data.' + self.member_name(name, m)
                    expression = self.expression(name, c['count'][1], 'neutral')
                    body.append('\tif (static_cast<std::int64_t>(%s) != (%s ? static_cast<std::int64_t>(%s->size()) : 0))\n\t{\n'
                                '\t\tthrow std::runtime_error("%s.%s has a length that its count does not give");\n\t}\n'
                                % (expression, field, field, neutral, self.member_name(name, m)))
            for count, arrays in self.count_groups(name).items():
                limit = self.count_limit(name, count)
                names = ['data.' + self.member_name(name, a) for a in arrays]
                checks = ['(%s && %s->size() > %s)' % (n, n, limit) for n in names]
                for other in names[1:]:
                    checks.append('(!%s != !%s)' % (names[0], other))
                    checks.append('(%s && %s && %s->size() != %s->size())' % (names[0], other, names[0], other))
                body.append('\tif (%s)\n\t{\n\t\tthrow std::runtime_error("%s has arrays that its count does not fit");\n\t}\n'
                            % (' || '.join(checks), neutral))
            for asset in self.spec['assets']:
                if asset['struct'] == name:
                    body.insert(0, self.root_name_check(asset))
            lines.append('inline void validate(const %s& data)\n{' % neutral)
            lines.extend(body or ['\t(void) data;'])
            lines += ['}', '']
        return lines

    def root_name_check(self, asset):
        """The check that an asset has a name: the spec gives the Replay member path to it."""
        struct, expr, kind = asset['struct'], 'data', None
        for part in asset['name'].split('.'):
            m, c = next((m, c) for m, c in self.structs[struct] if m['name'] == part)
            expr += '.' + self.member_name(struct, m)
            kind, struct = c['kind'], m['type']
        if kind == 'string':
            return ('\tif (!%s)\n\t{\n\t\tthrow std::runtime_error("%s has no name");\n\t}\n\n\tdetail::name(*%s, "%s");\n'
                    % (expr, asset['neutral'], expr, asset['neutral']))
        return '\tdetail::name(%s, "%s");\n' % (expr, asset['neutral'])

    def count_limit(self, name, count):
        for m, c in self.structs[name]:
            if m['name'] == count:
                bits = {'std::uint8_t': 0xFF, 'std::uint16_t': 0xFFFF, 'std::int32_t': 0x7FFFFFFF, 'std::uint32_t': 0xFFFFFFFF,
                        'std::int16_t': 0x7FFF, 'std::int8_t': 0x7F}
                return '0x%Xull' % bits.get(self.scalar_type(m['type']), 0xFFFFFFFF)
        return '0xFFFFFFFFull'

    # ---- the adapter -----------------------------------------------------------------------------
    def native_type(self, name):
        return name

    def read_expr(self, owner, m, c):
        field = 'native.' + m['name']
        what = '"%s.%s"' % (owner, m['name'])
        kind = c['kind']
        if kind == 'scalar':
            if m['dims']:
                if len(m['dims']) > 1:
                    return 'plain::read_block<%s>(&%s)' % (self.neutral_type(m, c), field)
                return 'std::to_array(%s)' % field
            return field if m['type'] in SCALARS else 'static_cast<%s>(%s)' % (self.scalar_type(m['type']), field)
        if kind == 'text':
            return 'plain::read_text(%s, %s)' % (field, what)
        if kind == 'string':
            return 'plain::read_string(%s)' % field
        if kind == 'script_string':
            return 'plain::read_script_string(%s)' % field
        if kind == 'flags':
            return 'plain::flag_names(asset_data::%s, static_cast<std::uint64_t>(%s), %s)' % (self.enum_table(m['type'])[0], field, what)
        if kind == 'enum':
            return 'plain::enum_name(asset_data::%s, static_cast<std::int64_t>(%s), %s)' % (self.enum_table(m['type'])[0], field, what)
        if kind == 'vector':
            if m['dims']:
                return self.loop_read(m, 'plain::read_scalars<%s, %d>(&%s[i])' % (c['scalar'], c['size'], field), self.neutral_type(m, c))
            return 'plain::read_scalars<%s, %d>(&%s)' % (c['scalar'], c['size'], field)
        if kind == 'overlay':
            return 'plain::read_scalars<std::uint%d_t, 1>(&%s)[0]' % (c['bits'], field)
        if kind == 'struct':
            reader = 'read_%s' % self.struct_name(m['type'])
            if m['dims']:
                return self.loop_read(m, '%s(%s[i])' % (reader, field), self.neutral_type(m, c))
            return '%s(%s)' % (reader, field)
        if kind == 'asset':
            return 'plain::read_asset(%s, %s)' % (c['asset'], field)
        if kind == 'array':
            count = c['count']
            if count == ('literal', 1):
                return 'plain::read_single(%s, [&](const auto& item) { return %s; })' % (field, self.element_read(c['element'], 'item'))
            if count[0] == 'literal':
                return 'plain::read_fixed<%s, %d>(%s)' % (self.element_type(c['element']), count[1], field)
            native_count = 'native.' + count[1] if count[0] == 'member' else self.expression(owner, count[1], 'native')
            if c.get('group'):
                return ('plain::read_array(reinterpret_cast<const std::array<%s, %d>*>(%s), %s, %s, '
                        '[&](const auto& item) { return plain::read_block<%s>(&item); })') % (
                    c['element'], c['group'], field, native_count, what, self.neutral_type(m, c)[len('std::optional<std::vector<'):-2])
            return 'plain::read_array(%s, %s, %s, [&](const auto& item) { return %s; })' % (
                field, native_count, what, self.element_read(c['element'], 'item'))
        raise SystemExit('no read for %s' % kind)

    def expression(self, owner, text, side):
        """A count expression of the stream rules over the native members (side native) or the neutral fields."""
        members = {m['name']: (m, c) for m, c in self.structs[owner]}

        def member(found):
            name = found.group(1)
            if side == 'native':
                return 'native.' + name + found.group(2)
            m, c = members[name]
            if 'counts' in c:
                # A count that an array derives has no field: it is the length of that array.
                array = 'data.' + self.member_name(owner, members[c['counts'][0]][0])
                return '(%s ? %s->size() : std::size_t{})' % (array, array)
            return 'data.' + self.member_name(owner, m) + self.path_names(m['type'], found.group(2))
        return re.sub(r'\b\w+(?:\[\w+\])?\.(\w+)((?:\.\w+)*)', member, text)

    def path_names(self, struct, rest):
        """The neutral names of the members that follow the first one of a path."""
        out = ''
        for name in [p for p in rest.split('.') if p]:
            m, _ = next((m, c) for m, c in self.structs[struct] if m['name'] == name)
            out += '.' + self.member_name(struct, m)
            struct = m['type']
        return out

    def element_read(self, type_name, item):
        if type_name == 'scr_string_t':
            return 'plain::read_script_string(%s)' % item
        if type_name == 'char*':
            return 'plain::read_string(%s)' % item
        vector = self.vector_of(type_name)
        if vector:
            return 'plain::read_scalars<%s, %d>(&%s)' % (vector[0], vector[1], item)
        if type_name in self.source.structs:
            return 'read_%s(%s)' % (self.struct_name(type_name), item)
        return item if type_name in SCALARS else 'static_cast<%s>(%s)' % (self.scalar_type(type_name), item)

    def loop_read(self, m, element, result_type):
        return '[&] { %s out; for (std::size_t i = 0; i < out.size(); ++i) { out[i] = %s; } return out; }()' % (result_type, element)

    def write_stmts(self, owner, m, c):
        field = 'native.' + m['name']
        data = 'data.' + self.member_name(owner, m)
        what = '"%s.%s"' % (owner, m['name'])
        kind = c['kind']
        if kind == 'scalar':
            if len(m['dims']) > 1:
                return ['plain::write_block(&%s, %s);' % (field, data)]
            if m['dims']:
                return ['std::ranges::copy(%s, %s);' % (data, field)]
            return ['%s = %s;' % (field, data if m['type'] in SCALARS else 'static_cast<%s>(%s)' % (m['type'], data))]
        if kind == 'text':
            return ['plain::write_text(%s, %s, %s);' % (field, data, what)]
        if kind == 'string':
            return ['%s = plain::write_string(memory, %s);' % (field, data)]
        if kind == 'script_string':
            return ['%s = static_cast<scr_string_t>(plain::write_script_string(%s));' % (field, data)]
        if kind == 'flags':
            return ['%s = static_cast<%s>(plain::flag_value(asset_data::%s, %s, %s));' % (
                field, m['type'], self.enum_table(m['type'])[0], data, what)]
        if kind == 'enum':
            return ['%s = static_cast<%s>(plain::enum_value(asset_data::%s, %s, %s));' % (
                field, m['type'], self.enum_table(m['type'])[0], data, what)]
        if kind == 'vector':
            if m['dims']:
                return ['for (std::size_t i = 0; i < %s.size(); ++i)' % data, '{',
                        '\tplain::write_scalars(&%s[i], %s[i]);' % (field, data), '}']
            return ['plain::write_scalars(&%s, %s);' % (field, data)]
        if kind == 'overlay':
            return ['plain::write_scalars(&%s, std::array<std::uint%d_t, 1>{%s});' % (field, c['bits'], data)]
        if kind == 'struct':
            writer = 'write_%s' % self.struct_name(m['type'])
            if m['dims']:
                return ['for (std::size_t i = 0; i < %s.size(); ++i)' % data, '{',
                        '\t%s(%s[i], %s[i], resolve, memory);' % (writer, field, data), '}']
            return ['%s(%s, %s, resolve, memory);' % (writer, field, data)]
        if kind == 'asset':
            return ['%s = plain::resolve_asset<%s>(resolve, %s, %s);' % (field, c['target'], c['asset'], data)]
        if kind == 'array':
            count = c['count']
            native_element = c['native_element']
            if c.get('group'):
                return ['%s = reinterpret_cast<%s*>(plain::write_array<std::array<%s, %d>>(%s, memory, '
                        '[&](auto& out, const auto& in) { plain::write_block(&out, in); }));' % (
                            field, native_element, native_element, c['group'], data)]
            if count == ('literal', 1):
                return ['%s = plain::write_single<%s>(%s, memory, [&](auto& out, const auto& in) { %s });' % (
                    field, native_element, data, self.element_write('out', 'in', c['element']))]
            if count[0] == 'literal':
                return ['if (%s)' % data, '{',
                        '\t%s = memory.manual_allocate<%s>(plain::rounded(%d * sizeof(%s)));' % (field, native_element, count[1], native_element),
                        '\tfor (std::size_t i = 0; i < %d; ++i)' % count[1], '\t{',
                        '\t\t%s' % self.element_write('%s[i]' % field, '(*%s)[i]' % data, c['element']), '\t}', '}']
            return ['%s = plain::write_array<%s>(%s, memory, [&](auto& out, const auto& in) { %s });' % (
                field, native_element, data, self.element_write('out', 'in', c['element']))]
        raise SystemExit('no write for %s' % kind)

    def element_write(self, out, source, type_name):
        if type_name == 'scr_string_t':
            return '%s = static_cast<scr_string_t>(plain::write_script_string(%s));' % (out, source)
        if type_name == 'char*':
            return '%s = plain::write_string(memory, %s);' % (out, source)
        if self.vector_of(type_name):
            return 'plain::write_scalars(&%s, %s);' % (out, source)
        if type_name in self.source.structs:
            return 'write_%s(%s, %s, resolve, memory);' % (self.struct_name(type_name), out, source)
        return '%s = %s;' % (out, source if type_name in SCALARS else 'static_cast<%s>(%s)' % (type_name, source))

    def size_stmts(self, owner, m, c):
        data = 'data.' + self.member_name(owner, m)
        kind = c['kind']
        if kind == 'string':
            return ['size += plain::string_size(%s);' % data]
        if kind == 'struct' and m['type'] in self.structs:
            if m['dims']:
                return ['for (const auto& item : %s)' % data, '{', '\tsize += size_%s(item);' % self.struct_name(m['type']), '}']
            return ['size += size_%s(%s);' % (self.struct_name(m['type']), data)]
        if kind == 'array':
            count = c['count']
            native_element = c['native_element']
            if c.get('group'):
                return ['if (%s)' % data, '{', '\tsize += plain::rounded(std::max<std::size_t>(%s->size(), 1) * %d * sizeof(%s));' % (
                    data, c['group'], native_element), '}']
            if count == ('literal', 1):
                lines = ['if (%s)' % data, '{', '\tsize += plain::rounded(sizeof(%s));' % native_element]
                if c['element'] in self.structs:
                    lines.append('\tsize += size_%s(*%s);' % (self.struct_name(c['element']), data))
                return lines + ['}']
            if count[0] == 'literal':
                return ['size += %s ? plain::rounded(%d * sizeof(%s)) : 0;' % (data, count[1], native_element)]
            lines = ['if (%s)' % data, '{', '\tsize += plain::rounded(std::max<std::size_t>(%s->size(), 1) * sizeof(%s));' % (data, native_element)]
            if c['element'] in self.structs:
                lines += ['\tfor (const auto& item : *%s)' % data, '\t{', '\t\tsize += size_%s(item);' % self.struct_name(c['element']), '\t}']
            if c['element'] == 'char*':
                lines += ['\tfor (const auto& item : *%s)' % data, '\t{', '\t\tsize += plain::string_size(item);', '\t}']
            return lines + ['}']
        return []

    def adapter(self):
        self.qual = 'asset_data::'
        family = self.spec['family']
        out = ['// Emitted by tools/gen_iw8_neutral.py from tools/neutral_families/%s.json: do not edit.' % family,
               '#include <std_include.hpp>', '#include "%s.hpp"' % family, '#include "plain.hpp"', '',
               'namespace zonetool::iw8::neutral', '{', '\tnamespace', '\t{']
        for name in self.order():
            out.extend(self.adapter_functions(name))
        out.append('\t}')
        out.append('')
        for asset in self.spec['assets']:
            out.extend(self.root_functions(asset))
        out.append('}')
        return '\n'.join(out) + '\n'

    def indented(self, lines, tabs):
        return [('\t' * tabs + line) if line else line for line in lines]

    def adapter_functions(self, name):
        neutral = 'asset_data::' + self.struct_name(name)
        short = self.struct_name(name)
        members = self.structs[name]
        read = ['%s read_%s(const %s& native)' % (neutral, short, name), '{', '\t%s data;' % neutral]
        write = ['void write_%s(%s& native, const %s& data, [[maybe_unused]] const asset_resolver& resolve, '
                 '[[maybe_unused]] plain::context& memory)' % (short, name, neutral), '{']
        size = ['std::size_t size_%s([[maybe_unused]] const %s& data)' % (short, neutral), '{', '\tstd::size_t size = 0;']
        for m, c in members:
            if c['kind'] == 'padding':
                read.append('\tplain::require_zero(native.%s, "%s.%s");' % (m['name'], name, m['name']))
            elif c['kind'] == 'unknown':
                raise SystemExit('unknown member %s.%s' % (name, m['name']))
            elif 'counts' in c:
                continue
            else:
                read.append('\tdata.%s = %s;' % (self.member_name(name, m), self.read_expr(name, m, c)))
                write.extend(self.indented(self.write_stmts(name, m, c), 1))
                size.extend(self.indented(self.size_stmts(name, m, c), 1))
        # The counts: set from the first array that uses them.
        for count, arrays in self.count_groups(name).items():
            count_member = next(m for m, _ in members if m['name'] == count)
            first = self.member_name(name, arrays[0])
            write.append('\tnative.%s = plain::count_of<%s>(data.%s);' % (count, count_member['type'], first))
        for m, c in members:
            if 'counts' in c and c['kind'] not in ('array',):
                pass
        read += ['\treturn data;', '}', '']
        write += ['}', '']
        size += ['\treturn size;', '}', '']
        return self.indented(read + write + size, 2)

    def root_functions(self, asset):
        native, neutral = asset['struct'], 'asset_data::' + asset['neutral']
        return [
            '\tstd::expected<%s, std::string> to_neutral(const %s& native)' % (neutral, native), '\t{', '\t\ttry', '\t\t{',
            '\t\t\tauto data = read_%s(native);' % asset['neutral'], '\t\t\tasset_data::validate(data);', '\t\t\treturn data;', '\t\t}',
            '\t\tcatch (const std::exception& error)', '\t\t{', '\t\t\treturn std::unexpected(error.what());', '\t\t}', '\t}', '',
            '\t%s* from_neutral(const %s& data, const asset_resolver& resolve, zone_memory& memory)' % (native, neutral), '\t{',
            '\t\tasset_data::validate(data);', '\t\tplain::context context(memory);',
            '\t\tauto* native = context.allocate<%s>();' % native,
            '\t\twrite_%s(*native, data, resolve, context);' % asset['neutral'], '\t\treturn native;', '\t}', '',
            '\tstd::size_t native_size(const %s& data)' % neutral, '\t{',
            '\t\treturn plain::rounded(sizeof(%s)) + size_%s(data) + 64;' % (native, asset['neutral']), '\t}', '']

    def adapter_header(self):
        family = self.spec['family']
        out = ['#pragma once', '', '// Emitted by tools/gen_iw8_neutral.py from tools/neutral_families/%s.json: do not edit.' % family,
               '#include <core/assets/%s_data.hpp>' % family, '#include "resolver.hpp"', '#include "../structs.hpp"', '',
               '#include <expected>', '#include <string>', '', 'namespace zonetool', '{', '\tclass zone_memory;', '}', '',
               '// Replay 1.20 %s assets to and from neutral data (docs/NEUTRAL_ASSETS.md).' % family,
               'namespace zonetool::iw8::neutral', '{']
        for asset in self.spec['assets']:
            native, neutral = asset['struct'], 'asset_data::' + asset['neutral']
            out += ['\tstd::expected<%s, std::string> to_neutral(const %s& native);' % (neutral, native),
                    '\t%s* from_neutral(const %s& data, const asset_resolver& resolve, zone_memory& memory);' % (native, neutral),
                    '\tstd::size_t native_size(const %s& data);' % neutral, '']
        out.append('}')
        return '\n'.join(out) + '\n'

    def root_validators(self):
        return []

    # ---- the plan --------------------------------------------------------------------------------
    def plan(self):
        lines = []
        for name, members in self.structs.items():
            lines.append('struct %s -> %s' % (name, self.struct_name(name)))
            for m, c in members:
                detail = c['kind']
                if c['kind'] == 'array':
                    detail += ' of %s by %s' % (m['type'], c['count'])
                elif c['kind'] == 'asset':
                    detail += ' ' + c['asset']
                elif c['kind'] == 'enum':
                    detail += ' ' + m['type']
                elif 'counts' in c:
                    detail = 'count of ' + ', '.join(c['counts'])
                lines.append('    %-26s %s' % (m['name'], detail))
        lines.append('')
        lines.append('problems: %d' % len(self.problems))
        lines.extend('  ' + p for p in self.problems)
        return '\n'.join(lines)


def main():
    args = [a for a in sys.argv[1:] if not a.startswith('--')]
    if not args:
        sys.exit(__doc__)
    family = Family(json.load(open(args[0], encoding='utf-8')))
    print(family.plan())
    if '--plan' in sys.argv:
        return
    if family.problems:
        sys.exit('the family has problems; nothing was written')
    name = family.spec['family']
    outputs = {
        os.path.join(ROOT, 'src', 'core', 'assets', name + '_data.hpp'): family.header(),
        os.path.join(ROOT, 'src', 'games', 'iw8', 'neutral', name + '.hpp'): family.adapter_header(),
        os.path.join(ROOT, 'src', 'games', 'iw8', 'neutral', name + '.cpp'): family.adapter(),
    }
    for path, text in outputs.items():
        with open(path, 'w', encoding='utf-8', newline='') as handle:
            handle.write(text)
        print('wrote', os.path.relpath(path, ROOT))


if __name__ == '__main__':
    main()

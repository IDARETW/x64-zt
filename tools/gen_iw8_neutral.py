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
        self.enum_names = {}
        for name, base, body in re.findall(r'\n\tenum (\w+) : ([\w:]+)\n\t\{\n((?:\t\t[^\n]*\n)*)\t\};', text):
            values = collections.OrderedDict()
            for member, value in re.findall(r'^\t\t(\w+) = (-?\d+),?$', body, re.M):
                values.setdefault(int(value), member)
            self.enums[name] = (base, values)
            self.enum_names[name] = {member: int(value) for member, value in re.findall(r'^		(\w+) = (-?\d+),?$', body, re.M)}

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


# `*reinterpret_cast<const T*>(&data)` or `(&data.member)`: the first scalar of a struct, read through a cast.
CAST = r'\*reinterpret_cast<const ([\w:]+)\*>\(&\w+(?:\[\w+\])?(?:\.(\w+(?:\.\w+)*))?\)'


class Rules:
    """What each pointer member is, from the stream traversals."""

    def __init__(self, source):
        self.source = source
        text = open(STREAMS, encoding='utf-8').read()
        self.ptr_array_assets = {}
        for helper in re.finditer(r'void (stream_\w+_ptr_array)\(Stream& stream, [^)]*\)\n\t\t\{\n(.*?)\n\t\t\}', text, re.S):
            asset = re.search(r'stream\.asset\((ASSET_TYPE_\w+), data\[', helper.group(2))
            if asset:
                self.ptr_array_assets[helper.group(1)] = asset.group(1)
        self.strings = set()
        self.assets = {}
        self.arrays = {}
        self.element_sizes = {}
        self.asset_arrays = {}
        self.overlay_pointers = set()
        self.referenced = set()
        # The rules of the members of the object a traversal function is about, by that object's struct: a member name alone
        # is ambiguous (`anims` is 22 pointers in one struct and 614 in another).
        self.owner_arrays = {}
        self.indexed = {}
        self.indexed2 = {}
        headers =[(h.start(), h.group(1)) for h in re.finditer(r'void stream_\w+\(Stream& stream, (\w+)& data\)', text)]
        self.streamed_structs = {name for _, name in headers}
        member = r'(?:\w+(?:\[\w+\])?\.)*?(\w+)'
        for m in re.finditer(r'stream\.string\(' + member + r'(?:\[\w+\])?\);', text):
            self.strings.add(m.group(1))
        for m in re.finditer(r'stream\.asset\((ASSET_TYPE_\w+), ' + member + r'\);', text):
            self.assets[m.group(2)] = m.group(1)
        # `stream_x_ptr(stream, data.member)`: a pointer to one struct, streamed by a helper that takes the pointer.
        for call in re.finditer(r'stream_\w+_ptr\(stream, data\.(\w+)\);', text):
            function = [name for start, name in headers if start < call.start()]
            self.arrays.setdefault(call.group(1), [])
            if ('literal', 1) not in self.arrays[call.group(1)]:
                self.arrays[call.group(1)].append(('literal', 1))
            if function:
                self.owner_arrays.setdefault((function[-1], call.group(1)), [])
                if ('literal', 1) not in self.owner_arrays[(function[-1], call.group(1))]:
                    self.owner_arrays[(function[-1], call.group(1))].append(('literal', 1))
        # `stream_x_ptr_array(stream, &data.member[0], 2)`: each element of the pointer array is one struct.
        for call in re.finditer(r'stream_(\w+)_ptr_array\(stream, &data\.(\w+)\[0\], (\d+)\);', text):
            if 'stream_%s_ptr_array' % call.group(1) in self.ptr_array_assets:
                continue
            for index in range(int(call.group(3))):
                entry = self.indexed.setdefault((call.group(2), index), [])
                if ('literal', 1) not in entry:
                    entry.append(('literal', 1))
        for call in re.finditer(r'stream\.(?:inline_array|array|elements)\(', text):
            arguments = self.arguments(text, call.end())
            if len(arguments) < 3:
                continue
            owner = re.search(r'(\w+)(?:\[[^\]]*\])*$', arguments[0])
            # The first argument is a member of some object: a bare name is the function's own array.
            if not owner or '.' not in arguments[0] or not re.match(r'^[\w]+(?:\[[^\]]*\])*(?:\.\w+(?:\[[^\]]*\])*)*$', arguments[0]):
                continue
            field, count = owner.group(1), arguments[-1].strip()
            if re.fullmatch(r'\d+', count):
                rule = ('literal', int(count))
            elif re.fullmatch(r'\w+(?:\[\w+\])?\.(\w+)', count):
                rule = ('member', re.fullmatch(r'\w+(?:\[\w+\])?\.(\w+)', count).group(1))
            elif re.fullmatch(CAST, count):
                shape = re.fullmatch(CAST, count)
                rule = ('first', shape.group(1), shape.group(2))
            elif len(set(re.findall(CAST, count))) == 1 and re.fullmatch(r'[@\d\s+*()<>-]*', re.sub(CAST, '@', count)):
                # Arithmetic over one first-scalar read (`((first + 31) >> 5) * first`): the cast becomes @.
                cast = re.search(CAST, count)
                rule = ('first_expr', cast.group(1), re.sub(CAST, '@', count), cast.group(2))
            elif 'outer_' not in count and 'reinterpret_cast' not in count and re.search(r'\.\w+', count):
                rule = ('expr', count)
            elif 'outer_' in count:
                rule = ('outer', count)
            else:
                continue
            if rule not in self.arrays.setdefault(field, []):
                self.arrays[field].append(rule)
            # `data.x[1]`: the rule of one element of a pointer array that has a count of its own for each element.
            element = re.search(r'\[(\d+)\]$', arguments[0])
            if element and rule not in self.indexed.setdefault((field, int(element.group(1))), []):
                self.indexed[(field, int(element.group(1)))].append(rule)
            element2 = re.search(r'\[(\d+)\]\[(\d+)\]$', arguments[0])
            if element2 and rule not in self.indexed2.setdefault((field, int(element2.group(1)), int(element2.group(2))), []):
                self.indexed2[(field, int(element2.group(1)), int(element2.group(2)))].append(rule)
            if arguments[0].startswith('data.') and re.fullmatch(r'data\.\w+', arguments[0]):
                function = [name for start, name in headers if start < call.start()]
                if function and rule not in self.owner_arrays.setdefault((function[-1], field), []):
                    self.owner_arrays[(function[-1], field)].append(rule)
            if rule[0] != 'outer':
                self.referenced.update(re.findall(r'\.(\w+)', count))
                self.referenced.add(count)
            # `if (const auto items_N = stream.inline_array(...))` followed by a helper over the pointers, or a loop of asset calls
            variable = re.search(r'const auto (items_\d+) = $', text[max(0, call.start() - 40):call.start()])
            if variable:
                following = text[call.end():call.end() + 700]
                helper = re.search(r'stream_(\w+?)_ptr_array\(stream, ' + variable.group(1) + ',', following)
                direct = re.search(r'stream\.asset\((ASSET_TYPE_\w+), ' + variable.group(1) + r'\[', following)
                if direct:
                    self.asset_arrays[field] = direct.group(1)
                elif helper and ('stream_%s_ptr_array' % helper.group(1)) in self.ptr_array_assets:
                    self.asset_arrays[field] = self.ptr_array_assets['stream_%s_ptr_array' % helper.group(1)]
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

    def plain_scalar(self, m):
        """A member whose value can stand in a count expression: a number, or an enum that counts are taken from."""
        return not m['pointer'] and not m['dims'] and (
            m['type'] in SCALARS or (m['type'] in self.source.enums and m['name'] in self.referenced))

    def leading_scalar(self, members):
        """The members that lead to the scalar at offset 0 of a struct: the first member, or through nested structs."""
        path = []
        while members:
            first = members[0]
            path.append(first)
            if self.plain_scalar(first):
                return path
            # An array of scalars starts with its first element.
            if len(first['dims']) == 1 and not first['pointer'] and first['type'] in SCALARS:
                path[-1] = dict(first, name=first['name'] + '[0]', dims=[])
                return path
            if first['pointer'] or first['dims'] or first['type'] not in self.source.structs or \
                    self.source.structs[first['type']][0] != 'struct':
                return None
            members = Source.members(self.source.structs[first['type']][1])
        return None

    def expression_applies(self, text, members):
        """True when every member path of a count expression exists in the owner and ends in a plain scalar."""
        for first, rest, index in re.findall(r'\b\w+(?:\[\w+\])?\.(\w+)((?:\.\w+)*)(\[\d+\])?', text):
            current = members
            names = [first] + [p for p in rest.split('.') if p]
            for depth, name in enumerate(names):
                found = next((m for m in current if m['name'] == name), None)
                if not found:
                    return False
                if depth == len(names) - 1:
                    # An element of an array of scalars (`counts[1]`) stands in for a scalar.
                    if index and len(found['dims']) == 1 and not found['pointer'] and found['type'] in SCALARS:
                        continue
                    if not self.plain_scalar(found):
                        return False
                elif found['pointer'] or found['dims'] or found['type'] not in self.source.structs or \
                        self.source.structs[found['type']][0] != 'struct':
                    return False
                else:
                    current = Source.members(self.source.structs[found['type']][1])
        return True

    def first_scalar_path(self, members, via):
        """The members that lead to the first scalar of the struct, or of its member `via`."""
        if not via:
            return self.leading_scalar(members)
        path, current = [], members
        names = via.split('.')
        for depth, name in enumerate(names):
            member = next((m for m in current if m['name'] == name), None)
            if not member or member['pointer']:
                return None
            if depth == len(names) - 1 and self.plain_scalar(member):
                return path + [member]
            if depth == len(names) - 1 and len(member['dims']) == 1 and member['type'] in SCALARS:
                return path + [dict(member, name=member['name'] + '[0]', dims=[])]
            if member['dims'] or member['type'] not in self.source.structs or self.source.structs[member['type']][0] != 'struct':
                return None
            path.append(member)
            current = Source.members(self.source.structs[member['type']][1])
        inner = self.leading_scalar(current)
        return path + inner if inner else None

    OUTER_CAST = r'\*reinterpret_cast<const ([\w:]+)\*>\(&outer_\w+(?:\.(\w+(?:\.\w+)*))?\)'
    DATA_CAST = r'\*reinterpret_cast<const ([\w:]+)\*>\(&data(?:\.(\w+(?:\.\w+)*))?\)'

    def outer_count(self, text, members):
        """A count over the members of the struct being read and of the struct that holds it, as native C++ over `native` and
        `outer`; None when the text names more than one outer struct or a path that is not a scalar."""
        names = set(re.findall(r'outer_(\w+)', text))
        if len(names) != 1:
            return None
        lower = names.pop()
        outer = next((n for n, (kind, _) in self.source.structs.items() if kind == 'struct' and n.lower() == lower), None)
        if not outer:
            return None
        outer_members = Source.members(self.source.structs[outer][1])

        def first(owner_members, prefix):
            def replacer(found):
                path = self.first_scalar_path(owner_members, found.group(2))
                if not path:
                    raise ValueError
                return prefix + '.'.join(p['name'] for p in path)
            return replacer
        try:
            text = re.sub(self.OUTER_CAST, first(outer_members, 'outer.'), text)
            text = re.sub(self.DATA_CAST, first(members, 'native.'), text)
        except ValueError:
            return None
        text = re.sub(r'\bouter_\w+\.', 'outer.', text)
        text = re.sub(r'\bdata\.', 'native.', text)
        return None if 'reinterpret_cast<const' in text and '&' in text else ('native', text, outer)

    def array_count(self, field, members, owner=None, index=None, outer_ok=False, index2=None):
        """The count rule of an array member: the first candidate that the owner can satisfy."""
        names = {m['name'] for m in members}
        # What the traversal of the owner's own struct says comes before what other structs say about the same name.
        candidates = self.owner_arrays.get((owner, field))
        if index2 is not None:
            candidates = self.indexed2.get((field, index, index2), [])
        elif index is not None:
            candidates = self.indexed.get((field, index), [])
        elif candidates is None:
            # A struct with a traversal of its own that names no rule for the member does not stream it.
            candidates = [] if owner in self.streamed_structs else self.arrays.get(field, [])
        # A literal count carries no check against the owner: it is tried last.
        for rule in sorted(candidates, key=lambda r: r[0] == 'literal'):
            if rule[0] == 'literal':
                return rule
            if rule[0] == 'member' and rule[1] in names:
                return rule
            if rule[0] in ('first', 'first_expr'):
                path = self.first_scalar_path(members, rule[-1] if rule[0] == 'first_expr' else rule[2])
                if path and path[-1]['type'].split('::')[-1] == rule[1].split('::')[-1]:
                    text = 'data.' + '.'.join(p['name'] for p in path)
                    return ('expr', text if rule[0] == 'first' else rule[2].replace('@', '(%s)' % text))
            if rule[0] == 'expr' and self.expression_applies(rule[1], members):
                return rule
            if rule[0] == 'outer' and outer_ok:
                resolved = self.outer_count(rule[1], members)
                if resolved:
                    return resolved
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
        self.path = []
        self.runtime = set(spec.get('runtime', []))
        self.spec_flag_enums = set(spec.get('flag_enums', []))
        self.spec_scalar_enums = set(spec.get('scalar_enums', []))
        self.blocks = spec.get('blocks', {})
        self.outer_counts = spec.get('outer_counts', False)
        self.count_overrides = spec.get('count_overrides', {})
        self.outer_needed = {}
        self.kept_counts = set(spec.get('kept_counts', []))
        self.data_members = set(spec.get('data_members', []))
        self.prefix_arrays = spec.get('prefix_arrays', {})
        self.problems = []
        self.synthetic = {}
        self.aliases = {}
        self.synthetic_member = {}
        self.tagged = {}
        for entry in spec.get('tagged', []):
            self.tagged[(entry['owner'], entry['member'])] = dict(entry, cases=self.dispatch_cases(entry))
        self.groups = spec.get('groups', {})
        self.owner_pointers = spec.get('owner_pointers', [])
        self.shared = set(spec.get('shared', []))
        self.borrowed = set(spec.get('borrowed', []))
        self.case_owner = {}
        for key, entry in self.tagged.items():
            for _, _, case_type in entry['cases']:
                self.case_owner[case_type] = key[0]
        self.external = spec.get('external', {})
        for asset in spec['assets']:
            self.visit(asset['struct'])

    # ---- names -----------------------------------------------------------------------------------
    def words(self, name):
        if name.endswith(']'):
            name = name[:name.index('[')] + '_' + name[name.index('[') + 1:-1]
        return '_'.join(self.token_names.get(t.lower(), t.lower()) for t in tokens(name))

    def struct_name(self, name):
        for asset in self.spec['assets']:
            if asset['struct'] == name:
                return asset['neutral']
        return self.struct_names.get(name) or self.words(name)

    def field_name(self, owner, name):
        return self.field_names.get(owner + '.' + name) or self.words(name)

    def dispatch_cases(self, entry):
        """The (tag value, union member, member type) of a tagged union.

        Each member is matched to the value of the tag enum whose name is the member's name in upper snake case under the
        spec's prefix (initSpawnShapeBox is PARTICLE_MODULE_INIT_SPAWN_SHAPE_BOX). The dispatch of the stream traversal, which
        only mentions the members that have something to stream, has to agree where it names a member."""
        union_type = next(m['type'] for m in Source.members(self.source.structs[entry['owner']][1]) if m['name'] == entry['member'])
        union_members = {m['name']: m['type'] for m in Source.members(self.source.structs[union_type][1])}
        tag_type = next(m['type'] for m in Source.members(self.source.structs[entry['owner']][1]) if m['name'] == entry['tag'])
        values = self.source.enum_names[tag_type]
        skip = set(entry.get('skip', []))
        cases = {}
        explicit = entry.get('cases')
        if explicit:
            union_all = {m['name']: m for m in Source.members(self.source.structs[union_type][1])}
            result = []
            for member, value in explicit.items():
                info = union_all[member]
                if isinstance(value, dict):
                    # Several tag values share a member: each gets a struct of its own, named by the spec, over the same native struct.
                    for number, alias in value.items():
                        self.aliases[alias] = info['type']
                        result.append((int(number), member, alias))
                    continue
                if info['pointer']:
                    synthetic = '%s__%s' % (entry['owner'], member)
                    self.synthetic[synthetic] = union_type
                    self.synthetic_member[synthetic] = dict(info, name=member)
                    result.append((int(value), member, synthetic))
                else:
                    result.append((int(value), member, info['type']))
            return sorted(result)
        for member, member_type in union_members.items():
            if member in skip:
                continue
            wanted = entry.get('aliases', {}).get(member, member).lower()
            found = next((number for name, number in values.items()
                          if name.startswith(entry['prefix']) and name[len(entry['prefix']):].replace('_', '').lower() == wanted), None)
            if found is None:
                self.problems.append('%s.%s: the tag enum %s has no name that reads as %s' % (entry['owner'], entry['member'], tag_type, member))
                continue
            cases[found] = member
        # the dispatch, where it names a member
        if not entry.get('dispatch'):
            return [(value, member, union_members[member]) for value, member in sorted(cases.items())]
        text = open(STREAMS, encoding='utf-8').read()
        first = text.index('void %s(Stream& stream' % entry['dispatch'])
        start = text.index('void %s(Stream& stream' % entry['dispatch'], first + 10)
        lines = text[start:text.index('\n\t\t}\n', start)].split('\n')
        for number, line in enumerate(lines):
            call = re.search(r'\.%s\.(\w+), ' % re.escape(entry['member']), line)
            if not call or call.group(1) not in union_members or call.group(1) in skip:
                continue
            indent = len(line) - len(line.lstrip('\t'))
            for back in range(number - 1, -1, -1):
                test = re.search(r'!= (\d+)\)$', lines[back])
                if test and len(lines[back]) - len(lines[back].lstrip('\t')) == indent - 1:
                    if cases.get(int(test.group(1))) != call.group(1):
                        self.problems.append('%s.%s: the dispatch puts %s at %s and the enum names do not' % (
                            entry['owner'], entry['member'], call.group(1), test.group(1)))
                    break
        return [(value, member, union_members[member]) for value, member in sorted(cases.items())]

    # ---- classification --------------------------------------------------------------------------
    def classify(self, owner, m, members):
        """The kind of a member: how to_neutral reads it and from_neutral writes it."""
        name, type_name = m['name'], m['type']
        # An element of an unrolled pointer array (x[1]) is looked up by the array's name and its index.
        rule_name, element = name, None
        if name.endswith(']'):
            rule_name, element = name[:name.index('[')], int(name[name.index('[') + 1:-1])
        prefix = self.prefix_arrays.get('%s.%s' % (owner, name))
        if prefix:
            return {'kind': 'array', 'element': 'asset|%s|%s' % (prefix['asset'], type_name), 'native_element': type_name + '*',
                    'count': ('member', prefix['count']), 'prefix': True}
        if type_name in self.external and not m['pointer']:
            return {'kind': 'external', 'ext': self.external[type_name]}
        if m['pointer'] == 1 and len(m['dims']) == 2 and element is None and self.mappable(type_name) and \
                sum((name, i, j) in self.rules.indexed2 for i in range(m['dims'][0]) for j in range(m['dims'][1])) >=                 m['dims'][0] * (m['dims'][1] - 1):
            # The last element of each row (like the last of 33) is not named by the stream and has no array.
            rows = [[self.count_of(owner, name, members, i, j) if (name, i, j) in self.rules.indexed2 else ('literal', 0)
                     for j in range(m['dims'][1])] for i in range(m['dims'][0])]
            if all(all(row) for row in rows):
                return {'kind': 'array', 'element': type_name, 'native_element': type_name, 'count': ('indexed2', rows)}
        if m['pointer'] == 1 and len(m['dims']) == 1 and element is None and self.mappable(type_name) and \
                sum((name, i) in self.rules.indexed for i in range(m['dims'][0])) >= m['dims'][0] - 1:
            # An element that the stream does not name (the last of 33) has no array.
            counts = [self.rules.array_count(name, members, owner, i) if (name, i) in self.rules.indexed else ('literal', 0)
                      for i in range(m['dims'][0])]
            if all(counts):
                return {'kind': 'array', 'element': type_name, 'native_element': type_name, 'count': ('indexed', counts)}
        tagged = self.tagged.get((owner, name))
        if tagged:
            return {'kind': 'tagged', 'tag': tagged['tag'], 'cases': tagged['cases']}
        if any(owner == o and name == t['tag'] for (o, _), t in self.tagged.items()):
            return {'kind': 'tag'}
        if PADDING.match(name) and '%s.%s' % (owner, name) not in self.data_members and '*.%s' % name not in self.data_members:
            return {'kind': 'padding'}
        if '%s.%s' % (owner, name) in self.runtime:
            return {'kind': 'runtime'}
        if type_name == 'scr_string_t' and not m['pointer']:
            if len(m['dims']) > 1:
                self.problems.append('%s.%s: a multidimensional array of script strings' % (owner, name))
                return {'kind': 'unknown'}
            return {'kind': 'script_string'}
        if m['pointer'] == 2 and type_name == 'char':
            count = self.count_of(owner, rule_name, members, element)
            if count:
                return {'kind': 'array', 'element': 'char*', 'native_element': 'const char*', 'count': count}
            return {'kind': 'padding'}
        if m['pointer'] == 1 and type_name == 'char':
            if rule_name in self.rules.strings:
                return {'kind': 'string'}
            count = self.count_of(owner, rule_name, members, element)
            if count:
                return {'kind': 'array', 'element': 'char', 'native_element': 'char', 'count': count}
            return {'kind': 'padding'}
        if m['pointer'] == 2 and rule_name in self.rules.asset_arrays:
            count = self.count_of(owner, rule_name, members, element)
            if count:
                return {'kind': 'array', 'element': 'asset|%s|%s' % (self.rules.asset_arrays[rule_name], type_name),
                        'native_element': type_name + '*', 'count': count}
        if m['pointer'] == 2 and rule_name not in self.rules.asset_arrays and type_name in self.source.structs and \
                self.source.structs[type_name][0] == 'struct':
            count = self.count_of(owner, rule_name, members, element)
            if count:
                return {'kind': 'array', 'element': 'ptr|%s' % type_name, 'native_element': type_name + '*', 'count': count}
        if m['pointer']:
            count = self.count_of(owner, rule_name, members, element)
            if rule_name in self.rules.assets:
                return {'kind': 'asset', 'asset': self.rules.assets[rule_name], 'target': type_name}
            if count and m['pointer'] == 1:
                if not self.mappable(type_name):
                    self.problems.append('%s.%s: an array of %s has a stream rule and no mapping' % (owner, name, type_name))
                    return {'kind': 'unknown'}
                result = {'kind': 'array', 'element': type_name, 'native_element': type_name, 'count': count}
                declared = self.declared_size(type_name)
                for streamed in self.rules.element_sizes.get(rule_name, ()):
                    if declared and streamed != declared:
                        group = self.groups.get(rule_name)
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
        if type_name in self.source.enums and self.source.enums[type_name][1] and name in self.rules.referenced:
            return {'kind': 'scalar'}
        if type_name in self.blocks and not m['pointer']:
            return {'kind': 'block', 'neutral': self.blocks[type_name]}
        if type_name in self.spec_scalar_enums:
            return {'kind': 'scalar'}
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

    def count_of(self, owner, name, members, element=None, element2=None):
        """The count rule of an array member; a struct whose count needs its outer struct is remembered."""
        override = self.count_overrides.get('%s.%s' % (owner, name))
        if override:
            count = ('native', override['native'], override['outer'])
        else:
            count = self.rules.array_count(name, members, owner, element, self.outer_counts, element2)
        if count and count[0] == 'native':
            self.outer_needed[owner] = count[2]
        return count

    def unrolled(self, m):
        """A pointer array (T* x[2]) whose elements have counts of their own in the stream is one member per element."""
        if m['pointer'] == 1 and len(m['dims']) == 1 and m['dims'][0] <= 8 and m['type'] != 'char' and                 all((m['name'], i) in self.rules.indexed for i in range(m['dims'][0])):
            return [dict(m, name='%s[%d]' % (m['name'], i), dims=[]) for i in range(m['dims'][0])]
        return [m]

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
        return type_name in self.external or type_name in self.blocks or (type_name in SCALARS or type_name in self.source.enums or type_name == 'scr_string_t' or type_name in VECTORS or
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
            if not a['pointer'] and not a['dims'] and type_name in self.source.structs and self.source.structs[type_name][0] == 'struct':
                inner = Source.members(self.source.structs[type_name][1])
                inner_sizes = [self.SIZES.get(ALIASES.get(i['type'], i['type'])) if not i['pointer'] and not i['dims'] and
                               i['type'] not in self.source.enums else None for i in inner]
                if inner and all(inner_sizes):
                    sizes.add(sum(inner_sizes))
                    continue
            if a['pointer'] and not a['dims'] and a['name'] in self.rules.overlay_pointers:
                sizes.add(8)
                continue
            if a['pointer'] or a['dims'] or type_name not in self.SIZES:
                self.problems.append('%s.%s: a union of %s is not mapped' % (owner, m['name'], m['type']))
                return {'kind': 'unknown'}
            sizes.add(self.SIZES[type_name])
        if not sizes or m['dims'] or max(sizes) not in (1, 2, 4, 8):
            self.problems.append('%s.%s: a union of %s has no scalar size or is an array' % (owner, m['name'], m['type']))
            return {'kind': 'unknown'}
        # Alternatives shorter than the longest share its first bytes: the bits of the longest hold all of them.
        return {'kind': 'overlay', 'bits': max(sizes) * 8, 'alternatives': [a['name'] for a in alternatives]}

    def native_of(self, name):
        """The Replay struct that an adapter struct reads and writes."""
        return self.synthetic.get(name) or self.aliases.get(name) or name

    def visit(self, name):
        if name in self.structs or name in self.blocks or self.vector_of(name) or (
                name not in self.source.structs and name not in self.synthetic and name not in self.aliases):
            return
        if name in self.aliases:
            kind, members = 'struct', Source.members(self.source.structs[self.aliases[name]][1])
        elif name in self.synthetic:
            kind, members = 'struct', [self.synthetic_member[name]]
        else:
            kind, body = self.source.structs[name]
            if kind == 'union':
                self.problems.append('%s: a union is not mapped' % name)
                return
            members = [e for m in Source.members(body) for e in self.unrolled(m)]
        classes = [self.classify(name, m, members) for m in members]
        for alias in self.owner_pointers:
            if self.case_owner.get(name) == alias['owner']:
                pointer = next((c for m, c in zip(members, classes) if m['name'] == alias['member']), None)
                if pointer is None or pointer.get('count') != ('literal', 1):
                    self.problems.append('%s.%s: an owner pointer is not a single object' % (name, alias['member']))
                    continue
                pointer['owner_member'] = alias['owner_member']
                members = members + [{'name': alias['flag'], 'type': 'bool', 'pointer': 0, 'dims': []}]
                classes = classes + [{'kind': 'owner_flag', 'pointer': alias['member']}]
        extras = []
        for m, c in zip(members, classes):
            if '%s.%s' % (name, m['name']) in self.shared or '%s.%s' % (name, m['name']) in self.borrowed:
                if c['kind'] != 'array' or c['count'][0] == 'literal':
                    self.problems.append('%s.%s: a shared member is not an array with a count' % (name, m['name']))
                    continue
                c['share_id'] = m['name'] + 'Id'
                c['borrowed'] = '%s.%s' % (name, m['name']) in self.borrowed
                extras.append(({'name': c['share_id'], 'type': 'std::uint32_t', 'pointer': 0, 'dims': []}, {'kind': 'share_id', 'array': m['name']}))
        members = members + [m for m, _ in extras]
        classes = classes + [c for _, c in extras]
        for m, c in zip(members, classes):
            if c.get('prefix'):
                m['dims'] = []
        self.structs[name] = list(zip(members, classes))
        self.path.append(name)
        for m, c in self.structs[name]:
            if c['kind'] == 'overlay':
                self.overlay_names[(name, m['name'])] = '_or_'.join(self.words(a) for a in c['alternatives'])
        # Counts that an array uses are derived from its length.
        counted = {}
        for m, c in self.structs[name]:
            if c['kind'] == 'array' and c['count'][0] == 'member':
                if '%s.%s' % (name, c['count'][1]) in self.kept_counts or '%s.*' % name in self.kept_counts or '*' in self.kept_counts or \
                        '%s.%s' % (name, m['name']) in self.borrowed:
                    c['kept_count'] = c['count'][1]
                    continue
                counted.setdefault(c['count'][1], []).append(m['name'])
        if '%s.*' % name in self.kept_counts or '*' in self.kept_counts or name in self.outer_needed:
            for m, c in self.structs[name]:
                if c['kind'] == 'array' and c['count'][0] in ('expr', 'native'):
                    c['kept_expr'] = True
        for m, c in self.structs[name]:
            if m['name'] in counted:
                c['counts'] = counted[m['name']]
        for m, c in self.structs[name]:
            if c['kind'] == 'array' and c['count'][0] == 'member' and not any(
                    n['name'] == c['count'][1] for n, _ in self.structs[name]):
                self.problems.append('%s.%s: its count %s is not in the struct' % (name, m['name'], c['count'][1]))
            if c['kind'] in ('struct',):
                self.visit(m['type'])
            if c['kind'] == 'tagged':
                for _, _, case_type in c['cases']:
                    self.visit(case_type)
            if c['kind'] == 'array' and c['element'].startswith('ptr|') and m['type'] in self.path:
                self.problems.append('%s.%s: pointers to %s make the struct contain itself' % (name, m['name'], m['type']))
            if c['kind'] == 'array' and m['type'] in self.source.structs and m['type'] not in self.external and m['type'] not in self.blocks and not c['element'].startswith('asset|'):
                self.visit(m['type'])
            if c['kind'] in ('enum', 'flags'):
                self.enums[m['type']] = self.source.enums[m['type']]
        self.path.pop()

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

    def owner_pointers_of(self, case_type):
        """(pointer member, its classification, flag member) of the case struct pointers that may be the owner's own member."""
        members = self.structs.get(case_type, [])
        return [(m, c, next(f for f, fc in members if fc['kind'] == 'owner_flag' and fc['pointer'] == m['name']))
                for m, c in members if 'owner_member' in c]

    def external_ns(self):
        return 'asset_data::' if self.qual else ''

    def family_ns(self):
        return 'asset_data::%s::' % self.spec['family']

    # The namespace prefix of neutral types: empty in the data header, the family namespace in the adapter.
    qual = ''

    def element_type(self, type_name):
        """The neutral type of an array element or an inline value of a type."""
        if type_name in ('scr_string_t', 'char*') or type_name.startswith('asset|'):
            return 'std::optional<std::string>'
        if type_name.startswith('ptr|'):
            return 'std::optional<%s>' % (self.qual + self.struct_name(type_name.split('|')[1]))
        if type_name in self.external:
            return self.external_ns() + self.external[type_name]['neutral']
        if type_name in self.blocks:
            return self.blocks[type_name]
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
        if kind == 'block':
            return self.nest(c['neutral'], m['dims'])
        if kind == 'owner_flag':
            return 'bool'
        if kind == 'share_id':
            return 'std::uint32_t'
        if kind == 'text':
            return 'std::string'
        if kind in ('string', 'script_string'):
            return self.nest('std::optional<std::string>', m['dims'])
        if kind == 'asset':
            return self.nest('std::optional<std::string>', m['dims'])
        if kind == 'enum':
            return 'std::string'
        if kind == 'flags':
            return 'std::vector<std::string>'
        if kind == 'external':
            return self.nest(self.external_ns() + c['ext']['neutral'], m['dims'])
        if kind == 'tagged':
            return 'std::variant<std::monostate, %s>' % ', '.join(self.qual + self.struct_name(t) for _, _, t in c['cases'])
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
            return self.nest('std::optional<std::vector<%s>>' % element, [] if c.get('prefix') else m['dims'])
        raise SystemExit('no neutral type for %s' % kind)

    def emitted(self, members):
        """The members that become neutral fields: not padding and not a count that an array derives."""
        return [(m, c) for m, c in members if c['kind'] not in ('padding', 'unknown', 'runtime', 'tag') and 'counts' not in c]

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
                by_value = c['kind'] == 'struct' or (c['kind'] == 'array' and (c['count'][0] == 'literal'))
                if by_value and m['type'] in self.structs and not c.get('element', '').startswith('asset|'):
                    visit(m['type'])
                if c['kind'] == 'tagged':
                    for _, _, case_type in c['cases']:
                        visit(case_type)
            out.append(name)
        for name in self.structs:
            visit(name)
        return out

    # ---- the data header -------------------------------------------------------------------------
    def header(self):
        family = self.spec['family']
        out = ['#pragma once', '', '#include "asset_data.hpp"'] + ['#include "%s"' % h for h in self.spec.get('data_includes', [])] + ['#include <algorithm>', '#include <array>', '#include <cstdint>', '#include <optional>',
               '#include <stdexcept>', '#include <string>', '#include <string_view>', '#include <type_traits>', '#include <utility>', '#include <variant>', '#include <vector>', '',
               '// Neutral %s assets (docs/NEUTRAL_ASSETS.md), emitted by tools/gen_iw8_neutral.py from the IW8 structs and stream' % family,
               '// traversals: do not edit; change the spec (tools/neutral_families/%s.json) or the generator.' % family,
               'namespace zonetool::asset_data', '{', 'namespace %s' % family, '{']
        for type_name in self.enums:
            table, names = self.enum_table(type_name)
            out.append('inline constexpr std::array<std::pair<std::int64_t, std::string_view>, %d> %s{{' % (len(names), table))
            out.extend('\t{%d, "%s"},' % (value, name) for value, name in names)
            out.append('}};')
            out.append('')
        out.extend('struct %s;' % self.struct_name(name) for name in self.structs)
        out.append('')
        out.extend('inline void validate(const %s& data);' % self.struct_name(name) for name in self.structs)
        out.append('')
        for name in self.order():
            out.extend(self.struct_decl(name))
        out.extend(self.validators())
        out.append('} // namespace %s' % family)
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
            if c['kind'] == 'array' and c['count'][0] == 'member' and not c.get('kept_count'):
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
                elif c['kind'] == 'external':
                    if m['dims']:
                        body.append('\tfor (const auto& item : %s)\n\t{\n\t\tvalidate(item);\n\t}\n' % field)
                    else:
                        body.append('\tvalidate(%s);\n' % field)
                elif c['kind'] == 'array' and c['element'] in self.external:
                    body.append('\tif (%s)\n\t{\n\t\tfor (const auto& item : *%s)\n\t\t{\n\t\t\tvalidate(item);\n\t\t}\n\t}\n' % (field, field))
                elif c['kind'] == 'tagged':
                    body.append('\tstd::visit([](const auto& value)\n\t{\n\t\tif constexpr (std::is_same_v<std::decay_t<decltype(value)>, std::monostate>)\n'
                                '\t\t{\n\t\t\tthrow std::runtime_error("%s.%s holds no module");\n\t\t}\n\t\telse\n\t\t{\n\t\t\tvalidate(value);\n\t\t}\n\t}, %s);\n'
                                % (neutral, self.member_name(name, m), field))
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
                elif c['kind'] == 'array' and c['element'].startswith('ptr|'):
                    body.append('\tif (%s)\n\t{\n\t\tfor (const auto& item : *%s)\n\t\t{\n\t\t\tif (item)\n\t\t\t{\n\t\t\t\tvalidate(*item);\n\t\t\t}\n\t\t}\n\t}\n' % (field, field))
                elif c['kind'] == 'array' and m['type'] in self.structs and not c['element'].startswith('asset|') and c['count'] == ('literal', 1):
                    body.append('\tif (%s)\n\t{\n\t\tvalidate(*%s);\n\t}\n' % (field, field))
                elif c['kind'] == 'array' and m['type'] in self.structs and not c['element'].startswith('asset|'):
                    body.append('\tif (%s)\n\t{\n\t\tfor (const auto& item : *%s)\n\t\t{\n\t\t\tvalidate(item);\n\t\t}\n\t}\n' % (field, field))
            for m, c in self.emitted(self.structs[name]):
                if c['kind'] == 'share_id':
                    array = next(x for x, _ in self.structs[name] if x['name'] == c['array'])
                    if '%s.%s' % (name, c['array']) in self.borrowed:
                        body.append('\tif (%s && %s == 0)\n\t{\n\t\tthrow std::runtime_error("%s has an array without an id");\n\t}\n'
                                    % ('data.' + self.member_name(name, array), 'data.' + self.member_name(name, m), neutral))
                        continue
                    body.append('\tif ((%s == 0) != !%s)\n\t{\n\t\tthrow std::runtime_error("%s has an id without its %s or the reverse");\n\t}\n'
                                % ('data.' + self.member_name(name, m), 'data.' + self.member_name(name, array), neutral, self.member_name(name, array)))
                if c['kind'] == 'owner_flag':
                    pointer = next(x for x, _ in self.structs[name] if x['name'] == c['pointer'])
                    body.append('\tif (data.%s && data.%s)\n\t{\n\t\tthrow std::runtime_error("%s has a %s that is both the member of its owner and a separate object");\n\t}\n'
                                % (self.member_name(name, m), self.member_name(name, pointer), neutral, self.member_name(name, pointer)))
                if c['kind'] == 'array' and c['count'][0] == 'indexed':
                    field = 'data.' + self.member_name(name, m)
                    for index, rule in enumerate(c['count'][1]):
                        body.append('\tif (%s[%d] && static_cast<std::int64_t>(%s) != static_cast<std::int64_t>(%s[%d]->size()))\n\t{\n'
                                    '\t\tthrow std::runtime_error("%s.%s has a length that its count does not give");\n\t}\n'
                                    % (field, index, self.count_text(name, rule, 'neutral'), field, index, neutral, self.member_name(name, m)))
                if c['kind'] == 'array' and c.get('kept_count'):
                    count = next(x for x, _ in self.structs[name] if x['name'] == c['kept_count'])
                    field = 'data.' + self.member_name(name, m)
                    body.append('\tif (%s && %s->size() != data.%s)\n\t{\n\t\tthrow std::runtime_error("%s.%s has a length that its count does not give");\n\t}\n'
                                % (field, field, self.member_name(name, count), neutral, self.member_name(name, m)))
                if c['kind'] == 'array' and c['count'][0] == 'expr':
                    field = 'data.' + self.member_name(name, m)
                    expression = self.expression(name, c['count'][1], 'neutral')
                    if c.get('kept_expr'):
                        body.append('\tif (%s && static_cast<std::int64_t>(%s) != static_cast<std::int64_t>(%s->size()))\n\t{\n'
                                    '\t\tthrow std::runtime_error("%s.%s has a length that its count does not give");\n\t}\n'
                                    % (field, expression, field, neutral, self.member_name(name, m)))
                        continue
                    if m['dims']:
                        body.append('\tfor (const auto& item : %s)\n\t{\n\t\tif (static_cast<std::int64_t>(%s) != (item ? static_cast<std::int64_t>(item->size()) : 0))\n\t\t{\n'
                                    '\t\t\tthrow std::runtime_error("%s.%s has a length that its count does not give");\n\t\t}\n\t}\n'
                                    % (field, expression, neutral, self.member_name(name, m)))
                        continue
                    body.append('\tif (static_cast<std::int64_t>(%s) != (%s ? static_cast<std::int64_t>(%s->size()) : 0))\n\t{\n'
                                '\t\tthrow std::runtime_error("%s.%s has a length that its count does not give");\n\t}\n'
                                % (expression, field, field, neutral, self.member_name(name, m)))
            for count, arrays in self.count_groups(name).items():
                limit = self.count_limit(name, count)
                names = ['data.' + self.member_name(name, a) + ('[0]' if a['dims'] else '') for a in arrays]
                for a in arrays:
                    if a['dims']:
                        whole = 'data.' + self.member_name(name, a)
                        checks_dims = ('std::ranges::any_of(%s, [&](const auto& item) { return !item != !%s[0] || (item && item->size() != %s[0]->size()); })'
                                       % (whole, whole, whole))
                        body.append('\tif (%s)\n\t{\n\t\tthrow std::runtime_error("%s.%s has arrays of different lengths");\n\t}\n'
                                    % (checks_dims, neutral, self.member_name(name, a)))
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

    def read_expr(self, owner, m, c, suffix=''):
        field = 'native.' + m['name'] + suffix
        what = '"%s.%s"' % (owner, m['name'])
        kind = c['kind']
        if kind == 'scalar':
            if m['dims']:
                if len(m['dims']) > 1 or m['type'] not in SCALARS:
                    return 'plain::read_block<%s>(&%s)' % (self.neutral_type(m, c), field)
                return 'std::to_array(%s)' % field
            return field if m['type'] in SCALARS else 'static_cast<%s>(%s)' % (self.scalar_type(m['type']), field)
        if kind == 'block':
            if m['dims']:
                return self.loop_read(m, 'plain::read_block<%s>(&%s[i])' % (c['neutral'], field), self.neutral_type(m, c))
            return 'plain::read_block<%s>(&%s)' % (c['neutral'], field)
        if kind == 'text':
            return 'plain::read_text(%s, %s)' % (field, what)
        if kind == 'string':
            if m['dims']:
                return self.loop_read(m, 'plain::read_string(%s[i])' % field, self.neutral_type(m, c))
            return 'plain::read_string(%s)' % field
        if kind == 'script_string':
            if m['dims']:
                return self.loop_read(m, 'plain::read_script_string(%s[i])' % field, self.neutral_type(m, c))
            return 'plain::read_script_string(%s)' % field
        if kind == 'external':
            if m['dims']:
                return self.loop_read(m, '%s(%s[i])' % (c['ext']['read'], field), self.neutral_type(m, c))
            return '%s(%s)' % (c['ext']['read'], field)
        if kind == 'tagged':
            variant = self.neutral_type(m, c)
            lines = ['[&]() -> %s {' % variant,
                     'switch (static_cast<std::uint32_t>(native.%s)) {' % c['tag']]
            for value, member, case_type in c['cases']:
                access = 'native.%s' % m['name'] if case_type in self.synthetic else 'native.%s.%s' % (m['name'], member)
                fixes = ''
                for pointer, info, flag in self.owner_pointers_of(case_type):
                    fixes += ' if (%s.%s == &native.%s) { item.%s.reset(); item.%s = true; }' % (
                        access, pointer['name'], info['owner_member'], self.member_name(case_type, pointer), self.member_name(case_type, flag))
                lines.append('case %d: { plain::module_scope scope(%d); auto item = read_%s(%s);%s return %s(std::move(item)); }' % (
                    value, value, self.struct_name(case_type), access, fixes, variant))
            lines.append('default: plain::fail("%s.%s", "has a tag value without a case"); } }()' % (owner, m['name']))
            return ' '.join(lines)
        if kind == 'flags':
            return 'plain::flag_names(%s%s, static_cast<std::uint64_t>(%s), %s)' % (self.family_ns(), self.enum_table(m['type'])[0], field, what)
        if kind == 'enum':
            return 'plain::enum_name(%s%s, static_cast<std::int64_t>(%s), %s)' % (self.family_ns(), self.enum_table(m['type'])[0], field, what)
        if kind == 'vector':
            if m['dims']:
                return self.loop_read(m, 'plain::read_scalars<%s, %d>(&%s[i])' % (c['scalar'], c['size'], field), self.neutral_type(m, c))
            return 'plain::read_scalars<%s, %d>(&%s)' % (c['scalar'], c['size'], field)
        if kind == 'overlay':
            return 'plain::read_scalars<std::uint%d_t, 1>(&%s)[0]' % (c['bits'], field)
        if kind == 'struct':
            reader = 'read_%s' % self.struct_name(m['type'])
            if m['type'] in self.outer_needed and not m['dims']:
                return '%s(%s, native)' % (reader, field)
            if m['dims']:
                return self.loop_read(m, '%s(%s[i])' % (reader, field), self.neutral_type(m, c))
            return '%s(%s)' % (reader, field)
        if kind == 'asset':
            if m['dims']:
                return self.loop_read(m, 'plain::read_asset(%s, %s[i])' % (c['asset'], field), self.neutral_type(m, c))
            return 'plain::read_asset(%s, %s)' % (c['asset'], field)
        if kind == 'array' and c.get('prefix'):
            return 'plain::read_prefix(%s, %s, %s, [&](const auto& item) { return %s; })' % (
                field, 'native.' + c['count'][1], what, self.element_read(c['element'], 'item'))
        if kind == 'array' and c['count'][0] == 'indexed2':
            table = ', '.join('{%s}' % ', '.join(self.count_text(owner, rule, 'native') for rule in row) for row in c['count'][1])
            return ('[&] { const std::size_t counts[][%d] = {%s}; %s out; for (std::size_t i = 0; i < out.size(); ++i) '
                    '{ for (std::size_t j = 0; j < out[i].size(); ++j) { out[i][j] = plain::read_array_if_present(%s[i][j], counts[i][j], %s, '
                    '[&](const auto& item) { return %s; }); } } return out; }()') % (
                len(c['count'][1][0]), table, self.neutral_type(m, c), field, what, self.element_read(c['element'], 'item'))
        if kind == 'array' and c['count'][0] == 'indexed':
            table = ', '.join(self.count_text(owner, rule, 'native') for rule in c['count'][1])
            return ('[&] { const std::size_t counts[] = {%s}; %s out; for (std::size_t i = 0; i < out.size(); ++i) '
                    '{ out[i] = plain::read_array_if_present(%s[i], counts[i], %s, [&](const auto& item) { return %s; }); } return out; }()') % (
                table, self.neutral_type(m, c), field, what, self.element_read(c['element'], 'item'))
        if kind == 'array' and m['dims'] and not suffix:
            return self.loop_read(m, self.read_expr(owner, m, c, '[i]'), self.neutral_type(m, c))
        if kind == 'array':
            count = c['count']
            if count == ('literal', 1):
                return 'plain::read_single(%s, [&](const auto& item) { return %s; })' % (field, self.element_read(c['element'], 'item'))
            if count[0] == 'literal':
                return 'plain::read_fixed_with<%d>(%s, [&](const auto& item) { return %s; })' % (
                    count[1], field, self.element_read(c['element'], 'item'))
            native_count = count[1] if count[0] == 'native' else (
                'native.' + count[1] if count[0] == 'member' else self.expression(owner, count[1], 'native'))
            if c.get('group'):
                return ('plain::read_array(reinterpret_cast<const std::array<%s, %d>*>(%s), %s, %s, '
                        '[&](const auto& item) { return plain::read_block<%s>(&item); })') % (
                    c['element'], c['group'], field, native_count, what, self.neutral_type(m, c)[len('std::optional<std::vector<'):-2])
            if c.get('borrowed'):
                return 'plain::read_borrowed(%s, %s, %s, [&](const auto& item) { return %s; })' % (
                    field, native_count, what, self.element_read(c['element'], 'item'))
            return 'plain::read_array%s(%s, %s, %s, [&](const auto& item) { return %s; })' % (
                '_if_present' if c.get('kept_count') or c.get('kept_expr') else '', field, native_count, what, self.element_read(c['element'], 'item'))
        raise SystemExit('no read for %s' % kind)

    def count_text(self, owner, rule, side):
        """A count rule as an expression over the native members (side native) or the neutral fields."""
        if rule[0] == 'literal':
            return str(rule[1])
        if rule[0] == 'member':
            return ('native.%s' % rule[1]) if side == 'native' else ('data.%s' % self.field_name(owner, rule[1]))
        return self.expression(owner, rule[1], side)

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
                first = members[c['counts'][0]][0]
                array = 'data.' + self.member_name(owner, first) + ('[0]' if first['dims'] else '')
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
        if type_name in self.external:
            return '%s(%s)' % (self.external[type_name]['read'], item)
        if type_name.startswith('ptr|'):
            return 'plain::read_pointee(%s, [&](const auto& pointee) { return read_%s(pointee); })' % (
                item, self.struct_name(type_name.split('|')[1]))
        if type_name.startswith('asset|'):
            return 'plain::read_asset(%s, %s)' % (type_name.split('|')[1], item)
        if type_name == 'scr_string_t':
            return 'plain::read_script_string(%s)' % item
        if type_name == 'char*':
            return 'plain::read_string(%s)' % item
        if type_name in self.blocks:
            return 'plain::read_block<%s>(&%s)' % (self.blocks[type_name], item)
        vector = self.vector_of(type_name)
        if vector:
            return 'plain::read_scalars<%s, %d>(&%s)' % (vector[0], vector[1], item)
        if type_name in self.source.structs:
            outer = ', native' if type_name in self.outer_needed else ''
            return 'read_%s(%s%s)' % (self.struct_name(type_name), item, outer)
        return item if type_name in SCALARS else 'static_cast<%s>(%s)' % (self.scalar_type(type_name), item)

    def loop_read(self, m, element, result_type):
        return '[&] { %s out; for (std::size_t i = 0; i < out.size(); ++i) { out[i] = %s; } return out; }()' % (result_type, element)

    def write_stmts(self, owner, m, c, suffix=''):
        field = 'native.' + m['name'] + suffix
        data = 'data.' + self.member_name(owner, m) + suffix
        what = '"%s.%s"' % (owner, m['name'])
        kind = c['kind']
        if kind == 'scalar':
            if len(m['dims']) > 1 or (m['dims'] and m['type'] not in SCALARS):
                return ['plain::write_block(&%s, %s);' % (field, data)]
            if m['dims']:
                return ['std::ranges::copy(%s, %s);' % (data, field)]
            return ['%s = %s;' % (field, data if m['type'] in SCALARS else 'static_cast<%s>(%s)' % (m['type'], data))]
        if kind == 'block':
            if m['dims']:
                return ['for (std::size_t i = 0; i < %s.size(); ++i)' % data, '{', '\tplain::write_block(&%s[i], %s[i]);' % (field, data), '}']
            return ['plain::write_block(&%s, %s);' % (field, data)]
        if kind == 'text':
            return ['plain::write_text(%s, %s, %s);' % (field, data, what)]
        if kind == 'string':
            if m['dims']:
                return ['for (std::size_t i = 0; i < %s.size(); ++i)' % data, '{', '\t%s[i] = plain::write_string(memory, %s[i]);' % (field, data), '}']
            return ['%s = plain::write_string(memory, %s);' % (field, data)]
        if kind == 'script_string':
            if m['dims']:
                return ['for (std::size_t i = 0; i < %s.size(); ++i)' % data, '{',
                        '\t%s[i] = static_cast<scr_string_t>(plain::write_script_string(%s[i]));' % (field, data), '}']
            return ['%s = static_cast<scr_string_t>(plain::write_script_string(%s));' % (field, data)]
        if kind == 'external':
            writer = c['ext']['write']
            if m['dims']:
                return ['for (std::size_t i = 0; i < %s.size(); ++i)' % data, '{',
                        '\t%s(%s[i], %s[i], resolve, memory);' % (writer, field, data), '}']
            return ['%s(%s, %s, resolve, memory);' % (writer, field, data)]
        if kind == 'tagged':
            tag_member = next(t for t, _ in self.structs[owner] if t['name'] == c['tag'])
            lines = ['std::visit([&](const auto& value)', '{', '\tusing value_type = std::decay_t<decltype(value)>;',
                     '\tif constexpr (std::is_same_v<value_type, std::monostate>)', '\t{',
                     '\t\tplain::fail("%s.%s", "holds no module");' % (owner, m['name']), '\t}']
            for value, member, case_type in c['cases']:
                neutral = self.qual + self.struct_name(case_type)
                lines += ['\telse if constexpr (std::is_same_v<value_type, %s>)' % neutral, '\t{',
                          '\t\tnative.%s = static_cast<%s>(%d);' % (c['tag'], tag_member['type'], value),
                          '\t\tplain::module_scope scope(%d);' % value,
                          '\t\twrite_%s(%s, value, resolve, memory);' % (self.struct_name(case_type),
                              'native.%s' % m['name'] if case_type in self.synthetic else 'native.%s.%s' % (m['name'], member))]
                for pointer, info, flag in self.owner_pointers_of(case_type):
                    lines += ['\t\tif (value.%s)' % self.member_name(case_type, flag), '\t\t{',
                              '\t\t\tnative.%s.%s.%s = &native.%s;' % (m['name'], member, pointer['name'], info['owner_member']), '\t\t}']
                lines.append('\t}')
            lines.append('}, %s);' % data)
            return lines
        if kind == 'flags':
            return ['%s = static_cast<%s>(plain::flag_value(%s%s, %s, %s));' % (
                field, m['type'], self.family_ns(), self.enum_table(m['type'])[0], data, what)]
        if kind == 'enum':
            return ['%s = static_cast<%s>(plain::enum_value(%s%s, %s, %s));' % (
                field, m['type'], self.family_ns(), self.enum_table(m['type'])[0], data, what)]
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
            if m['dims']:
                return ['for (std::size_t i = 0; i < %s.size(); ++i)' % data, '{',
                        '	%s[i] = plain::resolve_asset<%s>(resolve, %s, %s[i]);' % (field, c['target'], c['asset'], data), '}']
            return ['%s = plain::resolve_asset<%s>(resolve, %s, %s);' % (field, c['target'], c['asset'], data)]
        if kind == 'array' and c['count'][0] == 'indexed2':
            return ['for (std::size_t i = 0; i < data.%s.size(); ++i)' % self.member_name(owner, m), '{',
                    '\tfor (std::size_t j = 0; j < data.%s[i].size(); ++j)' % self.member_name(owner, m), '\t{',
                    '\t\t%s[i][j] = plain::write_array<%s>(%s[i][j], memory, [&](auto& out, const auto& in) { %s });' % (
                        field, c['native_element'], data, self.element_write('out', 'in', c['element'])), '\t}', '}']
        if kind == 'array' and c.get('prefix'):
            return ['plain::write_prefix(%s, %s, [&](auto& out, const auto& in) { %s });' % (
                field, data, self.element_write('out', 'in', c['element']))]
        if kind == 'array' and m['dims'] and not suffix:
            return (['for (std::size_t i = 0; i < data.%s.size(); ++i)' % self.member_name(owner, m), '{'] +
                    self.indented(self.write_stmts(owner, m, c, '[i]'), 1) + ['}'])
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
            if 'share_id' in c:
                return ['%s = plain::write_%s_array<%s>(data.%s, %s, memory, [&](auto& out, const auto& in) { %s });' % (
                    field, 'borrowed' if c.get('borrowed') else 'shared', native_element, self.member_name(owner, {'name': c['share_id']}), data,
                    self.element_write('out', 'in', c['element']))]
            return ['%s = plain::write_array<%s>(%s, memory, [&](auto& out, const auto& in) { %s });' % (
                field, native_element, data, self.element_write('out', 'in', c['element']))]
        raise SystemExit('no write for %s' % kind)

    def element_write(self, out, source, type_name):
        if type_name in self.external:
            return '%s(%s, %s, resolve, memory);' % (self.external[type_name]['write'], out, source)
        if type_name.startswith('ptr|'):
            target = type_name.split('|')[1]
            return '%s = plain::write_pointee<%s>(%s, memory, [&](auto& pointee, const auto& value) { write_%s(pointee, value, resolve, memory); });' % (
                out, target, source, self.struct_name(target))
        if type_name.startswith('asset|'):
            _, asset, target = type_name.split('|')
            return '%s = plain::resolve_asset<%s>(resolve, %s, %s);' % (out, target, asset, source)
        if type_name == 'scr_string_t':
            return '%s = static_cast<scr_string_t>(plain::write_script_string(%s));' % (out, source)
        if type_name == 'char*':
            return '%s = plain::write_string(memory, %s);' % (out, source)
        if type_name in self.blocks:
            return 'plain::write_block(&%s, %s);' % (out, source)
        if self.vector_of(type_name):
            return 'plain::write_scalars(&%s, %s);' % (out, source)
        if type_name in self.source.structs:
            return 'write_%s(%s, %s, resolve, memory);' % (self.struct_name(type_name), out, source)
        return '%s = %s;' % (out, source if type_name in SCALARS else 'static_cast<%s>(%s)' % (type_name, source))

    def size_stmts(self, owner, m, c, suffix=''):
        data = 'data.' + self.member_name(owner, m) + suffix
        kind = c['kind']
        if kind == 'array' and c['count'][0] == 'indexed2':
            return ['for (const auto& row : %s)' % data, '{', '\tfor (const auto& entry : row)', '\t{', '\t\tif (entry)', '\t\t{',
                    '\t\t\tsize += plain::sized(std::max<std::size_t>(entry->size(), 1) * sizeof(%s));' % c['native_element'], '\t\t}', '\t}', '}']
        if kind == 'array' and m['dims'] and not suffix and not c.get('prefix'):
            return (['for (std::size_t i = 0; i < data.%s.size(); ++i)' % self.member_name(owner, m), '{'] +
                    self.indented(self.size_stmts(owner, m, c, '[i]'), 1) + ['}'])
        if kind == 'external':
            if m['dims']:
                return ['for (const auto& item : %s)' % data, '{', '\tsize += %s(item);' % c['ext']['size'], '}']
            return ['size += %s(%s);' % (c['ext']['size'], data)]
        if kind == 'tagged':
            lines = ['std::visit([&](const auto& value)', '{', '\tusing value_type = std::decay_t<decltype(value)>;',
                     '\tif constexpr (std::is_same_v<value_type, std::monostate>)', '\t{', '\t}']
            for value, member, case_type in c['cases']:
                lines += ['\telse if constexpr (std::is_same_v<value_type, %s>)' % (self.qual + self.struct_name(case_type)), '\t{',
                          '\t\tsize += size_%s(value);' % self.struct_name(case_type), '\t}']
            lines.append('}, %s);' % data)
            return lines
        if kind == 'string':
            if m['dims']:
                return ['for (const auto& item : %s)' % data, '{', '\tsize += plain::string_size(item);', '}']
            return ['size += plain::string_size(%s);' % data]
        if kind == 'struct' and m['type'] in self.structs:
            if m['dims']:
                return ['for (const auto& item : %s)' % data, '{', '\tsize += size_%s(item);' % self.struct_name(m['type']), '}']
            return ['size += size_%s(%s);' % (self.struct_name(m['type']), data)]
        if kind == 'array':
            count = c['count']
            native_element = c['native_element']
            if c.get('group'):
                return ['if (%s)' % data, '{', '\tsize += plain::sized(std::max<std::size_t>(%s->size(), 1) * %d * sizeof(%s));' % (
                    data, c['group'], native_element), '}']
            if count == ('literal', 1):
                lines = ['if (%s)' % data, '{', '\tsize += plain::sized(sizeof(%s));' % native_element]
                if c['element'] in self.structs:
                    lines.append('\tsize += size_%s(*%s);' % (self.struct_name(c['element']), data))
                return lines + ['}']
            if count[0] == 'literal':
                lines = ['if (%s)' % data, '{', '	size += plain::sized(%d * sizeof(%s));' % (count[1], native_element)]
                if c['element'] in self.structs:
                    lines += ['	for (const auto& item : *%s)' % data, '	{', '		size += size_%s(item);' % self.struct_name(c['element']), '	}']
                if c['element'] in self.external:
                    lines += ['	for (const auto& item : *%s)' % data, '	{', '		size += %s(item);' % self.external[c['element']]['size'], '	}']
                return lines + ['}']
            lines = ['if (%s)' % data, '{', '\tsize += plain::sized(std::max<std::size_t>(%s->size(), 1) * sizeof(%s));' % (data, native_element)]
            if c['element'] in self.structs:
                lines += ['\tfor (const auto& item : *%s)' % data, '\t{', '\t\tsize += size_%s(item);' % self.struct_name(c['element']), '\t}']
            if c['element'] in self.external:
                lines += ['\tfor (const auto& item : *%s)' % data, '\t{', '\t\tsize += %s(item);' % self.external[c['element']]['size'], '\t}']
            if c['element'].startswith('ptr|'):
                target = c['element'].split('|')[1]
                lines += ['\tfor (const auto& item : *%s)' % data, '\t{', '\t\tif (item)', '\t\t{',
                          '\t\t\tsize += plain::sized(sizeof(%s)) + size_%s(*item);' % (target, self.struct_name(target)), '\t\t}', '\t}']
            if c['element'] == 'char*':
                lines += ['\tfor (const auto& item : *%s)' % data, '\t{', '\t\tsize += plain::string_size(item);', '\t}']
            return lines + ['}']
        return []

    def adapter(self):
        self.qual = self.family_ns()
        family = self.spec['family']
        out = ['// Emitted by tools/gen_iw8_neutral.py from tools/neutral_families/%s.json: do not edit.' % family,
               '#include <std_include.hpp>', '#include "%s.hpp"' % family, '#include "plain.hpp"'] + \
              ['#include "%s"' % h for h in self.spec.get('adapter_includes', [])] + ['',
               'namespace zonetool::iw8::neutral', '{', '\tnamespace', '\t{']
        for name in self.structs:
            short, native = self.struct_name(name), self.native_of(name)
            neutral = self.qual + short
            outer = ', const %s& outer' % self.outer_needed[name] if name in self.outer_needed else ''
            out.append('\t\t%s read_%s(const %s& native%s);' % (neutral, short, native, outer))
            out.append('\t\tvoid write_%s(%s& native, const %s& data, const asset_resolver& resolve, plain::context& memory);' % (short, native, neutral))
            out.append('\t\tstd::size_t size_%s(const %s& data);' % (short, neutral))
        out.append('')
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
        neutral = self.qual + self.struct_name(name)
        short = self.struct_name(name)
        members = self.structs[name]
        native_name = self.native_of(name)
        outer = ', [[maybe_unused]] const %s& outer' % self.outer_needed[name] if name in self.outer_needed else ''
        read = ['%s read_%s(const %s& native%s)' % (neutral, short, native_name, outer), '{', '\tplain::trace("read_%s");' % short,
                '\t%s data;' % neutral]
        write = ['void write_%s(%s& native, const %s& data, [[maybe_unused]] const asset_resolver& resolve, '
                 '[[maybe_unused]] plain::context& memory)' % (short, native_name, neutral), '{']
        size = ['std::size_t size_%s([[maybe_unused]] const %s& data)' % (short, neutral), '{', '\tstd::size_t size = 0;']
        for m, c in members:
            if c['kind'] in ('runtime', 'tag', 'owner_flag'):
                continue
            if c['kind'] == 'share_id':
                read.append('\tdata.%s = plain::pointer_ids::of(native.%s);' % (self.member_name(name, m), c['array']))
                continue
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
            first = self.member_name(name, arrays[0]) + ('[0]' if arrays[0]['dims'] else '')
            write.append('\tnative.%s = plain::count_of<%s>(data.%s);' % (count, count_member['type'], first))
        for m, c in members:
            if 'counts' in c and c['kind'] not in ('array',):
                pass
        read += ['\treturn data;', '}', '']
        write += ['}', '']
        size += ['\treturn size;', '}', '']
        return self.indented(read + write + size, 2)

    def root_functions(self, asset):
        native, neutral = asset['struct'], self.family_ns() + asset['neutral']
        return [
            '\tstd::expected<%s, std::string> to_neutral(const %s& native)' % (neutral, native), '\t{', '\t\ttry', '\t\t{',
            ] + (['\t\t\tplain::pointer_ids ids;'] if self.shared or self.borrowed else []) + [
            '\t\t\tauto data = read_%s(native);' % asset['neutral'], '\t\t\tvalidate(data);', '\t\t\treturn data;', '\t\t}',
            '\t\tcatch (const std::exception& error)', '\t\t{', '\t\t\treturn std::unexpected(error.what());', '\t\t}', '\t}', '',
            '\t%s* from_neutral(const %s& data, const asset_resolver& resolve, zone_memory& memory)' % (native, neutral), '\t{',
            '\t\tvalidate(data);', '\t\tplain::context context(memory);',
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
            native, neutral = asset['struct'], self.family_ns() + asset['neutral']
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

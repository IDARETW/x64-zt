"""Generate src/games/iw8/structs.hpp from the ACTS mw19 schema (Replay 1.20 profile).

Usage: python tools/gen_iw8_structs.py <acts data/mw19/schema.json> [output]

The output holds every type that the 117 asset pools reach, with the Replay
type overrides applied. static_asserts prove each size and member offset."""
import json, os, re, sys, collections

if len(sys.argv) < 2:
    sys.exit(__doc__)
SCHEMA = sys.argv[1]
OUT = sys.argv[2] if len(sys.argv) > 2 else 'src/games/iw8/structs.hpp'

schema = json.load(open(SCHEMA, encoding='utf-8'))
types = schema['types']
profile = schema['profiles']['replay-1.20']
overrides = dict(profile['type_overrides'])  # base name -> Replay name
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import iw8_layouts  # noqa: E402
iw8_layouts.merge(types, overrides, os.path.join(os.path.dirname(os.path.abspath(__file__)), 'iw8_layouts.txt'))
replay_to_base = {v: k for k, v in overrides.items()}

SCALARS = {
    'char': ('char', 1), 'signed char': ('std::int8_t', 1), 'unsigned char': ('std::uint8_t', 1),
    '__int8': ('std::int8_t', 1), 'unsigned __int8': ('std::uint8_t', 1), 'bool': ('bool', 1),
    'short': ('std::int16_t', 2), 'unsigned short': ('std::uint16_t', 2), '__int16': ('std::int16_t', 2),
    'unsigned __int16': ('std::uint16_t', 2), 'wchar_t': ('wchar_t', 2),
    'int': ('std::int32_t', 4), 'unsigned int': ('std::uint32_t', 4), 'long': ('std::int32_t', 4),
    'unsigned long': ('std::uint32_t', 4), '__int32': ('std::int32_t', 4), 'unsigned __int32': ('std::uint32_t', 4),
    'float': ('float', 4), 'double': ('double', 8), '__int64': ('std::int64_t', 8),
    'unsigned __int64': ('std::uint64_t', 8), 'long long': ('std::int64_t', 8),
    'unsigned long long': ('std::uint64_t', 8), 'void': ('void', 0),
}


def canonical(name):
    """Schema type name with Replay overrides folded into their base names."""
    if name in overrides:
        return name  # the base name stays; its definition comes from the override
    return replay_to_base.get(name, name)


def definition(name):
    """The schema entry that defines `name` under the Replay profile."""
    if name in overrides:
        return types[overrides[name]]
    return types.get(name)


def split_cv(name):
    """Leading cv-qualifiers of a scalar or of a type the schema has no entry for."""
    entry = definition(canonical(name))
    if canonical(name) in SCALARS or (entry is not None and entry['kind'] != 'scalar'):
        return '', name
    cv = ''
    while True:
        for q in ('const ', 'volatile '):
            if name.startswith(q):
                cv += q
                name = name[len(q):]
                break
        else:
            return cv, name


def ident(name):
    """A valid C++ identifier for a schema type name."""
    name = canonical(name)
    s = name.replace('::', '_').replace('<', '_').replace('>', '').replace(',', '_').replace(' ', '_')
    s = s.replace('*', 'ptr').replace('&', 'ref').replace('-', '_').replace('[', '_').replace(']', '')
    s = re.sub(r'[^A-Za-z0-9_]', '_', s)
    s = re.sub(r'_+', '_', s).strip('_')
    if s[0].isdigit():
        s = 't_' + s
    return s


# ---- reachability from the pool roots -------------------------------------
roots = [p['root_type'] for p in profile['pools'] if p.get('root_type')]
reach = []
seen = set()


def visit(name):
    cv, bare = split_cv(name)
    if cv:
        return visit(bare)
    name = canonical(name)
    if name in seen:
        return
    seen.add(name)
    entry = definition(name)
    if entry is None:
        return
    kind = entry['kind']
    if kind == 'pointer':
        visit(entry['target'])
    elif kind == 'array':
        visit(entry['element'])
    elif kind in ('struct', 'union'):
        for m in entry.get('members', []):
            t = m['type']
            if ':' in t and t.split(':')[0].strip() in SCALARS and t.split(':')[-1].strip().isdigit():
                continue  # bitfield
            visit(t)
    reach.append(name)


for r in roots:
    visit(r)

# ---- C++ type expressions ---------------------------------------------------
def is_bitfield(member):
    t = member['type']
    return re.match(r'^(.*\S)\s*:\s*(\d+)$', t) is not None and not t.endswith('::')


def type_info(name):
    """(kind, size, alignment) of a schema type under the Replay profile."""
    name = canonical(split_cv(name)[1])
    if name in SCALARS:
        size = SCALARS[name][1]
        return 'scalar', size, max(size, 1)
    entry = definition(name)
    if entry is None:
        raise KeyError(name)
    kind = entry['kind']
    if kind == 'pointer':
        return 'pointer', 8, 8
    if kind == 'array':
        _, esize, ealign = type_info(entry['element'])
        return 'array', entry['size'], ealign
    if kind == 'enum':
        return 'enum', entry['size'], entry['size']
    if kind == 'scalar':
        return 'scalar', entry['size'], entry['size']
    if kind == 'function':
        return 'function', 0, 1
    if kind == 'opaque':
        return 'opaque', entry['size'], 1
    return kind, entry['size'], alignment_of(name)


align_cache = {}


def alignment_of(name):
    name = canonical(name)
    if name in align_cache:
        return align_cache[name]
    entry = definition(name)
    a = 1
    if entry and entry['kind'] in ('struct', 'union'):
        for m in entry.get('members', []):
            if is_bitfield(m):
                base = re.match(r'^(.*\S)\s*:', m['type']).group(1)
                a = max(a, SCALARS.get(base, ('', 4))[1])
                continue
            try:
                a = max(a, type_info(m['type'])[2])
            except KeyError:
                a = max(a, 1)
    align_cache[name] = a
    return a


def cxx_type(name):
    """C++ spelling of a schema type used by value or behind a pointer."""
    cv, name = split_cv(name)
    if cv:
        return cv + cxx_type(name)
    name = canonical(name)
    if name in SCALARS:
        return SCALARS[name][0]
    entry = definition(name)
    if entry is None:
        if name.endswith('*'):
            return cxx_type(name[:-1].strip()) + '*'
        return 'std::uint8_t /* unknown ' + name + ' */'
    kind = entry['kind']
    if kind == 'pointer':
        target = canonical(entry['target'])
        tentry = definition(target)
        if target in SCALARS:
            spelled = SCALARS[target][0]
            if spelled == 'void':
                return 'void*'
            return spelled + '*'
        if tentry is not None and tentry['kind'] == 'scalar':
            return cxx_type(target) + '*'
        if tentry is None or tentry['kind'] == 'function':
            cv, bare = split_cv(target)
            if bare in SCALARS and bare != 'void':
                return cv + SCALARS[bare][0] + '*'
            return cv + 'void*'
        if tentry['kind'] == 'pointer':
            return cxx_type(target) + '*'
        if tentry['kind'] == 'array':
            return cxx_type(tentry['element']) + '*'
        return ident(target) + '*'
    if kind == 'array':
        return cxx_type(entry['element'])
    if kind in ('struct', 'union', 'enum', 'opaque'):
        return ident(name)
    if kind == 'scalar':
        return SCALARS.get(name, ('std::uint8_t', 1))[0]
    return 'std::uint8_t'


def array_suffix(name):
    name = canonical(name)
    entry = definition(name)
    dims = ''
    while entry and entry['kind'] == 'array':
        dims += '[%d]' % entry['count']
        entry = definition(canonical(entry['element']))
    return dims


def member_ident(name, used):
    s = re.sub(r'[^A-Za-z0-9_]', '_', name or 'unnamed')
    if not s or s[0].isdigit():
        s = 'm_' + s
    if s in ('this', 'class', 'struct', 'union', 'enum', 'default', 'delete', 'new', 'register', 'template', 'operator'):
        s += '_'
    base, n = s, 1
    while s in used:
        n += 1
        s = '%s_%d' % (base, n)
    used.add(s)
    return s


# ---- value dependencies for emission order ---------------------------------
def value_deps(name):
    entry = definition(name)
    out = []
    if entry and entry['kind'] in ('struct', 'union'):
        for m in entry.get('members', []):
            if is_bitfield(m) or m['size_bits'] == 0:
                continue
            t = canonical(m['type'])
            while definition(t) and definition(t)['kind'] == 'array':
                t = canonical(definition(t)['element'])
            e = definition(t)
            if e and e['kind'] in ('struct', 'union', 'enum', 'opaque'):
                out.append(t)
    return out


order, state = [], {}


def place(name):
    if state.get(name) == 2:
        return
    if state.get(name) == 1:
        raise RuntimeError('value cycle at ' + name)
    state[name] = 1
    for d in value_deps(name):
        place(d)
    state[name] = 2
    order.append(name)


emit_types = [n for n in reach if definition(n) and definition(n)['kind'] in ('struct', 'union', 'enum', 'opaque')]
for n in emit_types:
    place(n)

# ---- emission ----------------------------------------------------------------
lines = []
asserts = []
stats = collections.Counter()


def emit_enum(name, entry):
    size = entry['size']
    values = entry.get('values', {})
    signed = any(v < 0 for v in values.values())
    under = {1: 'std::int8_t' if signed else 'std::uint8_t', 2: 'std::int16_t' if signed else 'std::uint16_t',
             4: 'std::int32_t' if signed else 'std::uint32_t', 8: 'std::int64_t' if signed else 'std::uint64_t'}[size]
    lines.append('\tenum %s : %s' % (ident(name), under))
    lines.append('\t{')
    used = set()
    for k, v in sorted(values.items(), key=lambda kv: kv[1]):
        key = member_ident(k, used)
        lines.append('\t\t%s = %s,' % (key, str(v) if v >= 0 else '-%d' % -v))
    lines.append('\t};')
    lines.append('')
    asserts.append('\tstatic_assert(sizeof(%s) == %d);' % (ident(name), size))
    stats['enum'] += 1


def emit_record(name, entry):
    kind = entry['kind']
    size = entry['size']
    record = ident(name)
    # Empty base classes (EBO) are zero-size members in the schema.
    members = sorted((m for m in entry.get('members', []) if m['size_bits'] > 0),
                     key=lambda m: (m['offset_bits'], -m['size_bits']))
    natural_align = alignment_of(name)
    body = []
    used = set()
    offset = 0  # bytes, natural layout cursor
    checks = []
    packed = False
    if kind == 'union':
        for m in members:
            mt = m['type']
            if is_bitfield(m):
                base, bits = re.match(r'^(.*\S)\s*:\s*(\d+)$', mt).groups()
                body.append('\t\t%s %s : %s;' % (SCALARS.get(base, ('std::uint32_t', 4))[0], member_ident(m['name'], used), bits))
                continue
            body.append('\t\t%s %s%s;' % (cxx_type(mt), member_ident(m['name'], used), array_suffix(mt)))
        biggest = max([m['size_bits'] // 8 for m in members] or [0])
        if biggest < size:
            body.append('\t\tstd::uint8_t raw[%d];' % size)
    else:
        bit_unit = None  # (start byte, size bytes, used bits)
        for m in members:
            mt = m['type']
            byte_off = m['offset_bits'] // 8
            if is_bitfield(m):
                base, bits = re.match(r'^(.*\S)\s*:\s*(\d+)$', mt).groups()
                spelled, bsize = SCALARS.get(base, ('std::uint32_t', 4))
                unit_start = (m['offset_bits'] // (bsize * 8)) * bsize
                if bit_unit is None or bit_unit[0] != unit_start:
                    if unit_start > offset:
                        body.append('\t\tstd::uint8_t pad_%X[%d];' % (offset, unit_start - offset))
                        offset = unit_start
                    bit_unit = (unit_start, bsize, 0)
                    offset = unit_start + bsize
                lead = m['offset_bits'] - unit_start * 8 - bit_unit[2]
                if lead > 0:
                    body.append('\t\t%s : %d;' % (spelled, lead))
                body.append('\t\t%s %s : %s;' % (spelled, member_ident(m['name'], used), bits))
                bit_unit = (bit_unit[0], bit_unit[1], m['offset_bits'] - unit_start * 8 + int(bits))
                continue
            bit_unit = None
            _, msize, malign = type_info(mt)
            aligned = (offset + malign - 1) // malign * malign
            if byte_off < offset:
                packed = True  # overlapping or packed layout: keep going, the asserts decide
            if byte_off > aligned:
                body.append('\t\tstd::uint8_t pad_%X[%d];' % (offset, byte_off - offset))
                offset = byte_off
            elif byte_off != aligned:
                packed = True
            mname = member_ident(m['name'], used)
            body.append('\t\t%s %s%s;' % (cxx_type(mt), mname, array_suffix(mt)))
            checks.append((mname, byte_off))
            offset = byte_off + msize
        tail = (offset + natural_align - 1) // natural_align * natural_align
        if size > tail:
            body.append('\t\tstd::uint8_t pad_%X[%d];' % (offset, size - offset))
        elif size < tail:
            packed = True
    if packed:
        lines.append('#pragma pack(push, 1)')
        stats['packed'] += 1
    lines.append('\t%s %s' % (kind, record))
    lines.append('\t{')
    lines.extend(body)
    lines.append('\t};')
    if packed:
        lines.append('#pragma pack(pop)')
    lines.append('')
    asserts.append('\tstatic_assert(sizeof(%s) == %d);' % (record, size))
    for mname, off in checks:
        asserts.append('\tstatic_assert(offsetof(%s, %s) == 0x%X);' % (record, mname, off))
    stats[kind] += 1


def emit_opaque(name, entry):
    lines.append('\t// %s' % entry.get('reason', 'opaque in the Replay schema'))
    lines.append('\tstruct %s' % ident(name))
    lines.append('\t{')
    lines.append('\t\tstd::uint8_t data[%d];' % entry['size'])
    lines.append('\t};')
    lines.append('')
    asserts.append('\tstatic_assert(sizeof(%s) == %d);' % (ident(name), entry['size']))
    stats['opaque'] += 1


header = ['#pragma once', '', '// Generated from the ACTS mw19 schema, Replay 1.20 profile (game_dx12_ship_replay.exe 1.20.4.7623265):',
          '// every type that the 117 asset pools reach, with the Replay layout overrides applied.', '',
          'namespace zonetool::iw8', '{']
forward = []
for name in order:
    entry = definition(name)
    if entry['kind'] in ('struct', 'union'):
        forward.append('\t%s %s;' % (entry['kind'], ident(name)))
    elif entry['kind'] == 'opaque':
        forward.append('\tstruct %s;' % ident(name))
for name in order:
    entry = definition(name)
    k = entry['kind']
    if k == 'enum':
        emit_enum(name, entry)
    elif k == 'opaque':
        emit_opaque(name, entry)
    else:
        emit_record(name, entry)

text = '\n'.join(header + forward + [''] + lines + asserts + ['}', ''])
open(OUT, 'w', encoding='utf-8', newline='\r\n').write(text)
print('reachable', len(reach), 'emitted', dict(stats), 'lines', text.count('\n'))

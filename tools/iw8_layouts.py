"""Read tools/iw8_layouts.txt (Replay layout corrections) into schema-style type entries."""
import re


def parse(path):
    """Return (records, aliases). A record holds name, kind, size and members (offset, name, type)."""
    records = {}
    aliases = {}
    current = None
    for number, raw in enumerate(open(path, encoding='utf-8'), 1):
        line = raw.split('#', 1)[0].rstrip()
        if not line.strip():
            continue
        words = line.split()
        if not raw[0].isspace():
            if words[0] in ('struct', 'union') and len(words) == 3:
                current = {'name': words[1], 'kind': words[0], 'size': int(words[2], 0), 'members': []}
                records[words[1]] = current
                continue
            if words[0] == 'patch' and len(words) == 2:
                current = {'name': words[1], 'kind': 'patch', 'members': []}
                records[words[1]] = current
                continue
            if words[0] == 'alias' and len(words) == 3:
                aliases[words[1]] = words[2]
                current = None
                continue
            raise SyntaxError('%s:%d: unknown directive' % (path, number))
        match = re.match(r'\s+(\d+|0x[0-9A-Fa-f]+)\s+(\w+)\s*:\s*(.+)$', raw.rstrip('\n'))
        if not match or current is None:
            raise SyntaxError('%s:%d: bad member line' % (path, number))
        current['members'].append((int(match[1], 0), match[2], match[3].split('#', 1)[0].strip()))
    return records, aliases


SCALAR_SIZES = {
    'char': 1, 'signed char': 1, 'unsigned char': 1, '__int8': 1, 'unsigned __int8': 1, 'bool': 1,
    'short': 2, 'unsigned short': 2, '__int16': 2, 'unsigned __int16': 2, 'wchar_t': 2,
    'int': 4, 'unsigned int': 4, 'long': 4, 'unsigned long': 4, '__int32': 4, 'unsigned __int32': 4, 'float': 4,
    'double': 8, '__int64': 8, 'unsigned __int64': 8, 'long long': 8, 'unsigned long long': 8,
}


def clean(name):
    name = re.sub(r'\s+', ' ', name.strip())
    for prefix in ('const ', 'volatile '):
        while name.startswith(prefix):
            name = name[len(prefix):]
    return name


def resolve(name, overrides):
    name = clean(name)
    if overrides and name in overrides:
        return overrides[name]
    return name


def size_of(types, name, overrides=None):
    """Size in bytes of a schema type name."""
    name = clean(name)
    if name.endswith('*') or '(*)' in name:
        return 8
    match = re.match(r'(.*)\[(\d+)\]$', name)
    if match:
        return size_of(types, match[1], overrides) * int(match[2])
    if name in SCALAR_SIZES:
        return SCALAR_SIZES[name]
    return types[resolve(name, overrides)]['size']


def is_bitfield(member):
    return re.match(r'^(.*\S)\s*:\s*(\d+)$', member['type']) is not None and not member['type'].endswith('::')


def align_of(types, name, overrides=None, cache=None):
    """Natural alignment in bytes of a schema type name."""
    name = clean(name)
    if name.endswith('*') or '(*)' in name:
        return 8
    match = re.match(r'(.*)\[(\d+)\]$', name)
    if match:
        return align_of(types, match[1], overrides, cache)
    if name in SCALAR_SIZES:
        return SCALAR_SIZES[name]
    name = resolve(name, overrides)
    if cache is not None and name in cache:
        return cache[name]
    record = types[name]
    kind = record['kind']
    if kind in ('scalar', 'enum'):
        result = max(record['size'], 1)
    elif kind == 'pointer':
        result = 8
    elif kind == 'array':
        result = align_of(types, record['element'], overrides, cache)
    elif kind in ('struct', 'union'):
        result = 1
        for member in record.get('members', []):
            if is_bitfield(member):
                base = re.match(r'^(.*\S)\s*:', member['type']).group(1)
                result = max(result, SCALAR_SIZES.get(clean(base), 4))
                continue
            try:
                result = max(result, align_of(types, member['type'], overrides, cache))
            except KeyError:
                pass
    else:
        result = 1
    if cache is not None:
        cache[name] = result
    return result


def ensure_type(types, name, overrides):
    """Add schema entries for pointer and array spellings that only the corrections use."""
    name = re.sub(r'\s+', ' ', name.strip())
    if name in types or clean(name) in SCALAR_SIZES:
        return
    if name.endswith('*'):
        target = name[:-1].strip()
        ensure_type(types, target, overrides)
        types[name] = {'name': name, 'kind': 'pointer', 'size': 8, 'target': target}
        return
    match = re.match(r'(.*)\[(\d+)\]$', name)
    if match:
        element = match[1].strip()
        ensure_type(types, element, overrides)
        count = int(match[2])
        types[name] = {'name': name, 'kind': 'array', 'element': element, 'count': count,
                       'size': size_of(types, element, overrides) * count}


def merge(types, aliases_out, path, size_of=size_of):
    """Add the corrections to a schema type table and re-lay out the records that embed them.

    Returns (corrected record names, {re-laid out record: (schema size, new size)})."""
    records, aliases = parse(path)
    aliases_out.update(aliases)
    patches = {name: record for name, record in records.items() if record['kind'] == 'patch'}
    records = {name: record for name, record in records.items() if record['kind'] != 'patch'}
    for name, record in records.items():
        types[name] = {'name': name, 'kind': record['kind'], 'size': record['size'], 'members': []}
    for name, patch in patches.items():
        # A patch replaces the schema members that start at the listed offsets.
        base = types[name]
        offsets = {offset for offset, _, _ in patch['members']}
        kept = [dict(m) for m in base['members'] if m['offset_bits'] // 8 not in offsets]
        record = {'name': name, 'kind': base['kind'], 'size': base['size'], 'members': []}
        for m in kept:
            record['members'].append((m['offset_bits'] // 8, m['name'], m['type']))
        record['members'].extend(patch['members'])
        records[name] = record
        types[name] = {'name': name, 'kind': base['kind'], 'size': base['size'], 'members': []}
    for name, record in records.items():
        members = []
        for offset, member, type_name in record['members']:
            ensure_type(types, type_name, aliases_out)
            size = size_of(types, type_name, aliases_out)
            members.append({'name': member, 'offset_bits': offset * 8, 'size_bits': size * 8, 'type': type_name})
        members.sort(key=lambda m: m['offset_bits'])
        end = 0
        for m in members:
            if m['offset_bits'] < end and record['kind'] == 'struct':
                raise ValueError('%s: member %s overlaps the previous member' % (name, m['name']))
            end = m['offset_bits'] + m['size_bits']
        if end > record['size'] * 8:
            raise ValueError('%s: members end at %d, past the size %d' % (name, end // 8, record['size']))
        types[name]['members'] = members
    for base, replay in aliases.items():
        if replay not in types:
            raise ValueError('alias %s -> %s: unknown type' % (base, replay))
    relaid = relayout(types, aliases_out, set(records))
    return set(records), relaid


def relayout(types, overrides, fixed):
    """Re-lay out every record whose members changed size, until nothing changes.

    A record keeps its schema offsets while its members keep their schema sizes. When a member
    grows or shrinks, the record gets the natural MSVC layout: each member at the next offset
    aligned to its alignment, the size rounded up to the record alignment."""
    relaid = {}
    changed = True
    while changed:
        changed = False
        cache = {}
        for name, record in list(types.items()):
            if name in fixed or name in overrides or record.get('kind') not in ('struct', 'union'):
                continue
            members = record.get('members', [])
            if not members or any(is_bitfield(m) for m in members):
                continue
            try:
                # Empty bases (size 0) take no storage.
                sizes = [size_of(types, m['type'], overrides) * 8 if m['size_bits'] else 0 for m in members]
            except KeyError:
                continue
            if all(s == m['size_bits'] for s, m in zip(sizes, members)):
                continue
            new_members = []
            offset = 0
            alignment = 1
            for m, bits in zip(members, sizes):
                if not bits:
                    new_members.append(dict(m))
                    continue
                a = align_of(types, m['type'], overrides, cache)
                alignment = max(alignment, a)
                start = 0 if record['kind'] == 'union' else (offset + a - 1) // a * a
                new_members.append(dict(m, offset_bits=start * 8, size_bits=bits))
                offset = max(offset, start + bits // 8)
            size = (offset + alignment - 1) // alignment * alignment
            relaid[name] = (relaid.get(name, (record['size'],))[0], size)
            types[name] = dict(record, members=new_members, size=size)
            changed = True
    return relaid

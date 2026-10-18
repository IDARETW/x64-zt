"""Generate src/games/iw8/zone/asset_streams.{hpp,cpp} from the IW8 format spec.

Usage: python tools/gen_iw8_streams.py <acts data/mw19/schema.json> [spec.json] [output folder]

The spec (tools/iw8_format.json) describes what each Replay 1.20 Load_* routine reads, in member
terms. This script turns it into one traversal per record with nested data, and one entry point per
asset type. A traversal is a template over a stream (zone/streams.hpp): the same steps load, write,
dump and parse an asset, so the directions cannot disagree. The struct names and member names come
from gen_iw8_structs.py."""
import collections
import json
import os
import re
import runpy
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
if len(sys.argv) < 2:
    sys.exit(__doc__)
SCHEMA = sys.argv[1]
SPEC = sys.argv[2] if len(sys.argv) > 2 else os.path.join(HERE, 'iw8_format.json')
OUT = sys.argv[3] if len(sys.argv) > 3 else os.path.join(HERE, '..', 'src', 'games', 'iw8', 'zone')

saved_argv = sys.argv
sys.argv = [os.path.join(HERE, 'gen_iw8_structs.py'), SCHEMA, os.devnull]
S = runpy.run_path(os.path.join(HERE, 'gen_iw8_structs.py'), run_name='gen_iw8_structs')
sys.argv = saved_argv

spec = json.load(open(SPEC, encoding='utf-8'))
functions = spec['functions']
canonical, definition, ident, cxx_type = S['canonical'], S['definition'], S['ident'], S['cxx_type']
member_ident, is_bitfield, type_info = S['member_ident'], S['is_bitfield'], S['type_info']
SCALARS = S['SCALARS']


class Unsupported(Exception):
    pass


# ---- types and members -------------------------------------------------------------------------
def entry(name):
    name = canonical(S['split_cv'](name)[1])
    e = definition(name)
    if e is None and name.endswith('*'):
        return {'kind': 'pointer', 'size': 8, 'target': name[:-1].strip()}
    if e is None and '(*)' in name:
        # Pointer to an array: "T (*)[N]" points at T[N].
        return {'kind': 'pointer', 'size': 8, 'target': re.sub(r'\s*\(\*\)', '', name)}
    return e


def size_of(name):
    try:
        return type_info(name)[1]
    except KeyError:
        return None


member_cache = {}


def members(name):
    """Members of a record in emission order, with their C++ identifiers."""
    name = canonical(S['split_cv'](name)[1])
    if name in member_cache:
        return member_cache[name]
    e = definition(name)
    out = []
    if e and e['kind'] in ('struct', 'union'):
        used = set()
        ordered = sorted((m for m in e.get('members', []) if m['size_bits'] > 0),
                         key=lambda m: (m['offset_bits'], -m['size_bits']))
        for m in ordered:
            out.append(dict(m, cxx=member_ident(m['name'], used)))
    member_cache[name] = out
    return out


def find_member(type_name, member):
    for m in members(type_name):
        if m['name'] == member:
            return m
    raise Unsupported('%s has no member %s' % (type_name, member))


def pointer_target(type_name):
    e = entry(type_name)
    if e is None or e['kind'] != 'pointer':
        raise Unsupported('%s is not a pointer' % type_name)
    return canonical(S['split_cv'](e['target'])[1])


def array_element(type_name):
    e = entry(type_name)
    if e is not None and e['kind'] == 'array':
        return canonical(e['element']), e['count']
    return None, None


# ---- function forms ----------------------------------------------------------------------------
def walk(ops):
    for op in ops:
        yield op
        for key in ('body', 'then', 'else', 'ops'):
            if key in op:
                yield from walk(op[key])
        if op['op'] == 'switch':
            for case in op['cases']:
                yield from walk(case['ops'])


def is_count_size(expr):
    """True when a self-load size depends on the count parameter (an array loader)."""
    if expr is None:
        return False
    if 'param' in expr:
        return expr['param'] == 'count'
    return any(is_count_size(a) for a in expr.get('args', []))


forms = {}


def form(fid):
    """'string' | 'script_string' | 'script_strings' | 'asset' | 'struct' | 'array' | 'empty'."""
    if fid in forms:
        return forms[fid]
    f = functions[fid]
    ops = f['ops']
    all_ops = list(walk(ops))
    kinds = [o['op'] for o in all_ops]
    result = None

    def on_self(op):
        # The loader's own object (or one element of it, for arrays) holds the string.
        path = op['member'].get('path', [])
        return op['member']['root'] == 'self' and (not path or (len(path) == 1 and isinstance(path[0], dict)))
    if any(o['op'] == 'string_data' and on_self(o) for o in all_ops):
        result = 'strings' if is_count_size(f.get('self')) else 'string'
    elif any(o['op'] == 'script_string_data' and on_self(o) for o in all_ops) and \
            plain(f['type'] or '') == plain('scr_string_t'):
        result = 'script_strings' if is_count_size(f.get('self')) else 'script_string'
    elif f['type'] and f['type'].endswith('*') and any(
            o['op'] == 'pointer' and o['member']['root'] == 'self' and not o['member']['path'] and -3 in o['markers']
            for o in ops):
        result = 'asset'
    else:
        interesting = [o for o in all_ops if o['op'] not in ('data', 'hook', 'register', 'store')]
        if not interesting:
            result = 'empty'
        elif is_count_size(f.get('self')):
            result = 'array'
        else:
            result = 'struct'
    forms[fid] = result
    return result


def asset_type_of(fid):
    """Asset type id of an asset pointer loader: the asset_begin of the root loader it calls."""
    for op in walk(functions[fid]['ops']):
        if op['op'] == 'load' and op.get('fn'):
            for inner in walk(functions[op['fn']]['ops']):
                if inner['op'] == 'asset_begin' and 'const' in inner['type']:
                    return inner['type']['const']
    raise Unsupported('asset pointer %s without an asset type' % fid)


asset_names = {}
for asset in spec['assets']:
    asset_names[asset['id']] = asset


def asset_enum(type_id):
    return ASSET_ENUM[type_id]


# ---- names -------------------------------------------------------------------------------------
function_names = {}
used_names = set()


def function_name(fid):
    if fid not in function_names:
        f = functions[fid]
        base = 'stream_' + ident(f['type'] or 'unknown').lower()
        if form(fid) == 'array':
            base += '_array'
        name = base
        n = 1
        while name in used_names:
            n += 1
            name = '%s_%d' % (base, n)
        used_names.add(name)
        function_names[fid] = name
    return function_names[fid]


# ---- outer objects -----------------------------------------------------------------------------
def outers_of(fid, seen=None):
    """Outer object types (by loader var) that a function or its callees read."""
    seen = seen if seen is not None else set()
    if fid in seen:
        return set()
    seen.add(fid)
    out = set()

    def refs(node):
        if isinstance(node, dict):
            if node.get('root') == 'outer':
                out.add((node['var'], node['type']))
            for v in node.values():
                refs(v)
        elif isinstance(node, list):
            for v in node:
                refs(v)
    refs(functions[fid]['ops'])
    for op in walk(functions[fid]['ops']):
        if op['op'] == 'load' and op.get('fn') and form(op['fn']) in ('struct', 'array'):
            out |= outers_of(op['fn'], seen)
    own = functions[fid].get('var')
    return {o for o in out if o[0] != own}


# ---- code emission -----------------------------------------------------------------------------
class Context:
    def __init__(self, fid, self_expr, self_type, self_is_array, outers):
        self.fid = fid
        self.self_expr = self_expr
        self.self_type = self_type
        self.self_is_array = self_is_array
        self.item = None          # (expr, type, is_array) of the current pointer target
        self.loops = {}           # loop id -> index variable
        self.loop_counts = {}
        self.outers = outers      # var -> (expr, type)
        self.temps = {}
        self.indent = 2
        self.objects = []         # (expr, type) of enclosing loop elements and pointer targets
        self.required = collections.OrderedDict()  # var -> (parameter, type) the caller passes
        self.pending = []         # lines computing values that the next statement uses
        self.hoisted = {}         # expression key -> (variable, indent) of the computed values in scope
        self.sums = 0

    def flush(self, out):
        out.extend(self.pending)
        self.pending = []

    def line(self, text, out):
        # A line outside a block ends the scope of the values computed in it.
        self.hoisted = {k: v for k, v in self.hoisted.items() if v[1] <= self.indent}
        out.extend(self.pending)
        self.pending = []
        out.append('\t' * self.indent + text)


def literal(n):
    if n < 0:
        return '(%d)' % n
    if n > 0x7FFFFFFF:
        # 64-bit arithmetic, as in the loader (multiply-high divisions, masks).
        return '0x%Xull' % n
    return str(n) if n < 0x10000 else '0x%X' % n


def descend(expr, type_name, want):
    """Walk offset-0 members (a record's address is also its first member's address) until `want` holds."""
    seen = 0
    while not want(type_name):
        e = entry(type_name)
        if e is None or seen > 8:
            return None
        if e['kind'] in ('struct', 'union'):
            first = [m for m in members(type_name) if m['offset_bits'] == 0]
            if not first:
                return None
            if e['kind'] == 'union':
                for m in first:
                    found = descend('%s.%s' % (expr, m['cxx']), canonical(m['type']), want)
                    if found:
                        return found
                return None
            m = first[0]
            expr, type_name = '%s.%s' % (expr, m['cxx']), canonical(m['type'])
        elif e['kind'] == 'array':
            expr, type_name = '%s[0]' % expr, canonical(e['element'])
        else:
            return None
        seen += 1
    return expr, type_name


def access_as(ctx, ref, want):
    expr, type_name = access(ctx, ref)
    found = descend(expr, type_name, want)
    if found is None:
        raise Unsupported('%s (%s) has no member of the wanted type' % (expr, type_name))
    return found


def is_kind(*kinds):
    return lambda t: (entry(t) or {}).get('kind') in kinds


def plain(name):
    return re.sub(r'\b(const|volatile)\s+', '', canonical(name)).replace(' *', '*').replace('*', ' *').strip()


def is_type(name):
    target = plain(name)
    return lambda t: plain(t) == target


def access(ctx, ref):
    """C++ expression and schema type of a reference."""
    root = ref['root']
    if root == 'self':
        expr, type_name, is_array = ctx.self_expr, ctx.self_type, ctx.self_is_array
    elif root == 'item':
        if ctx.item is None:
            raise Unsupported('item reference outside a pointer')
        expr, type_name, is_array = ctx.item
    elif root == 'outer':
        expr, type_name = resolve_outer(ctx, ref['var'], ref['type'])
        is_array = False
    else:
        raise Unsupported('reference root %s' % root)
    path = list(ref.get('path', []))
    if is_array:
        if path and isinstance(path[0], dict) and 'index' in path[0]:
            expr = '%s[%s]' % (expr, ctx.loops.get(path[0]['index'], 'i%d' % path[0]['index']))
            path = path[1:]
        elif path and isinstance(path[0], int):
            expr = '%s[%d]' % (expr, path[0])
            path = path[1:]
        else:
            expr = '%s[0]' % expr
    if root == 'self' and getattr(ctx, 'skip_zero', False) and path and path[0] == 0:
        # A one-element array type loaded as an array of its element.
        path = path[1:]
    for part in path:
        if isinstance(part, str):
            m = find_member(type_name, part)
            expr = '%s.%s' % (expr, m['cxx'])
            type_name = canonical(m['type'])
        elif isinstance(part, int) or (isinstance(part, dict) and 'index' in part):
            stride = part.get('stride') if isinstance(part, dict) else None
            element, _ = array_element(type_name)
            if element is None and (entry(type_name) or {}).get('kind') == 'pointer':
                # Index the data a pointer member points to (read earlier by the same loader).
                element = pointer_target(type_name)
                element_of_target, _ = array_element(element)
                if element_of_target is not None:
                    element = element_of_target
            if element is None:
                # Index through a record or union whose first array member holds the elements.
                found = descend(expr, type_name, lambda t: array_element(t)[0] is not None and (
                    stride is None or size_of(array_element(t)[0]) == stride))
                if found is None:
                    raise Unsupported('index into %s' % type_name)
                expr, type_name = found
                element, _ = array_element(type_name)
            index = str(part) if isinstance(part, int) else ctx.loops.get(part['index'], 'i%d' % part['index'])
            expr = '%s[%s]' % (expr, index)
            type_name = element
        else:
            raise Unsupported('path part %r' % (part,))
    if 'byte_offset' in ref or 'remainder' in ref:
        raise Unsupported('reference into the middle of %s' % type_name)
    return expr, type_name


WIDTH_TYPES = {(1, False): 'std::uint8_t', (2, False): 'std::uint16_t', (4, False): 'std::uint32_t',
               (8, False): 'std::uint64_t', (1, True): 'std::int8_t', (2, True): 'std::int16_t',
               (4, True): 'std::int32_t', (8, True): 'std::int64_t'}

BINARY = {'+': '+', '-': '-', '*': '*', '&': '&', '|': '|', '^': '^', '>>': '>>', '<<': '<<', '/': '/', '%': '%'}


def expression(ctx, expr):
    if expr is None:
        raise Unsupported('missing expression')
    if 'const' in expr:
        return literal(expr['const'])
    if 'param' in expr:
        if expr['param'] == 'count':
            return 'count'
        raise Unsupported('parameter %s' % expr['param'])
    if 'field' in expr:
        ref = expr['field']
        if ref['root'] == 'temp':
            if ref['id'] not in ctx.temps:
                raise Unsupported('temp %s' % ref['id'])
            return ctx.temps[ref['id']]
        text, type_name = access(ctx, ref)
        width = expr['width']
        size = size_of(type_name)
        spelled = WIDTH_TYPES[(width, bool(expr.get('signed')))]
        kind = (entry(type_name) or {}).get('kind')
        if size == width and kind not in ('struct', 'union', 'array', 'opaque'):
            if kind in ('enum',) or type_name in ('bool', 'float'):
                return 'static_cast<%s>(%s)' % (spelled, text) if type_name != 'float' else \
                    'std::bit_cast<%s>(%s)' % (spelled, text)
            return text
        return '*reinterpret_cast<const %s*>(&%s)' % (spelled, text)
    if 'op' in expr:
        name, args = expr['op'], [expression(ctx, a) for a in expr['args']]
        if name in BINARY and len(args) == 2:
            return '(%s %s %s)' % (args[0], BINARY[name], args[1])
        if name == 'neg':
            return '(-%s)' % args[0]
        if name == 'not':
            return '(~%s)' % args[0]
        if name == 'popcnt':
            return 'std::popcount(static_cast<std::uint64_t>(%s))' % args[0]
        if name == 'sign':
            return '(static_cast<std::int64_t>(%s) < 0 ? -1 : 0)' % args[0]
        if name == 'sar' and len(args) == 2:
            return '(static_cast<std::int64_t>(%s) >> %s)' % (args[0], args[1])
        if name == 'mulhi' and len(args) == 2:
            return '__umulh(static_cast<std::uint64_t>(%s), static_cast<std::uint64_t>(%s))' % (args[0], args[1])
        raise Unsupported('operator %s' % name)
    if 'reduce' in expr:
        # A loop that sums a value per element: compute the sum before the statement that uses it.
        loop = expr['reduce']
        if loop not in ctx.loop_counts:
            raise Unsupported('reduce over an unknown loop')
        key = json.dumps(expr, sort_keys=True)
        found = ctx.hoisted.get(key)
        if found and found[1] <= ctx.indent:
            return found[0]
        index = 'i%d' % loop
        saved = ctx.loops.get(loop)
        ctx.loops[loop] = index
        count = expression(ctx, ctx.loop_counts[loop])
        start = expression(ctx, expr['start'])
        pending = len(ctx.pending)
        step = expression(ctx, expr['step'])
        if len(ctx.pending) != pending:
            raise Unsupported('a sum inside a sum')
        if saved is None:
            del ctx.loops[loop]
        else:
            ctx.loops[loop] = saved
        ctx.sums += 1
        name = 'sum_%d' % ctx.sums
        pad = '\t' * ctx.indent
        ctx.pending += [pad + 'std::size_t %s = %s;' % (name, start),
                        pad + 'for (std::size_t %s = 0; %s < %s; %s++)' % (index, index, count, index),
                        pad + '{', pad + '\t%s += %s;' % (name, step), pad + '}']
        ctx.hoisted[key] = (name, ctx.indent)
        return name
    if 'select' in expr:
        return '(%s ? %s : %s)' % (condition(ctx, expr['select']), expression(ctx, expr['then']),
                                   expression(ctx, expr['else']))
    if 'index' in expr:
        index = ctx.loops.get(expr['index'], 'i%d' % expr['index'])
        start, step = expr.get('start', 0), expr.get('step', 1)
        text = index if step == 1 else '%s * %d' % (index, step)
        return '(%d + %s)' % (start, text) if start else text
    raise Unsupported('expression %s' % json.dumps(expr)[:80])


CC = {'e': '==', 'z': '==', 'ne': '!=', 'nz': '!=', 'a': '>', 'b': '<', 'ae': '>=', 'be': '<=', 'g': '>', 'l': '<',
      'ge': '>=', 'le': '<='}


INVERSE = {'e': 'ne', 'z': 'nz', 'ne': 'e', 'nz': 'z', 'a': 'be', 'be': 'a', 'b': 'ae', 'ae': 'b', 'g': 'le',
           'le': 'g', 'l': 'ge', 'ge': 'l', 's': 'ns', 'ns': 's'}


def condition(ctx, cond, negate=False):
    if negate:
        cc = cond.get('compare', cond.get('test'))
        if cc in INVERSE:
            key = 'compare' if 'compare' in cond else 'test'
            return condition(ctx, dict(cond, **{key: INVERSE[cc]}))
        return '!(%s)' % condition(ctx, cond)
    if 'compare' in cond:
        cc = cond['compare']
        if cc not in CC:
            raise Unsupported('condition code %s' % cc)
        a, b = (expression(ctx, x) for x in cond['args'])
        width = cond.get('width') or 8
        if cc in ('g', 'l', 'ge', 'le'):
            spelled = WIDTH_TYPES[(width, True)]
            a, b = 'static_cast<%s>(%s)' % (spelled, a), 'static_cast<%s>(%s)' % (spelled, b)
        elif cc in ('a', 'b', 'ae', 'be'):
            spelled = WIDTH_TYPES[(width, False)]
            a, b = 'static_cast<%s>(%s)' % (spelled, a), 'static_cast<%s>(%s)' % (spelled, b)
        return '%s %s %s' % (a, CC[cc], b)
    if 'test' in cond:
        cc = cond['test']
        a, b = cond['args']
        if a == b:
            value = expression(ctx, a)
        else:
            value = '(%s & %s)' % (expression(ctx, a), expression(ctx, b))
        if cc in ('e', 'z'):
            return '%s == 0' % value
        if cc in ('ne', 'nz'):
            return '%s != 0' % value
        if cc == 's':
            return 'static_cast<std::int64_t>(%s) < 0' % value
        if cc == 'ns':
            return 'static_cast<std::int64_t>(%s) >= 0' % value
        raise Unsupported('test condition %s' % cc)
    raise Unsupported('condition %s' % json.dumps(cond)[:80])


def divide(expr, k):
    """expr / k for a size expression that is a multiple of k."""
    if 'select' in expr:
        return {'select': expr['select'], 'then': divide(expr['then'], k), 'else': divide(expr['else'], k)}
    terms = linear(expr)
    if terms is None:
        raise Unsupported('size is not linear')
    out = {}
    for key, coefficient in terms.items():
        if coefficient % k:
            raise Unsupported('size is not a multiple of %d' % k)
        out[key] = coefficient // k
    return rebuild(out)


def linear(expr):
    """{atom json: coefficient} for sums of products by constants."""
    if 'const' in expr:
        return {'1': expr['const']}
    if 'op' in expr and expr['op'] in ('+', '-') and len(expr['args']) == 2:
        a, b = linear(expr['args'][0]), linear(expr['args'][1])
        if a is None or b is None:
            return None
        sign = 1 if expr['op'] == '+' else -1
        for key, value in b.items():
            a[key] = a.get(key, 0) + sign * value
        return {k: v for k, v in a.items() if v}
    if 'op' in expr and expr['op'] == '*' and len(expr['args']) == 2:
        a, b = linear(expr['args'][0]), linear(expr['args'][1])
        if a is None or b is None:
            return None
        if list(b) == ['1']:
            return {k: v * b['1'] for k, v in a.items()}
        if list(a) == ['1']:
            return {k: v * a['1'] for k, v in b.items()}
        return {json.dumps(expr, sort_keys=True): 1}
    if 'op' in expr and expr['op'] == '<<' and 'const' in expr['args'][1]:
        a = linear(expr['args'][0])
        return None if a is None else {k: v << expr['args'][1]['const'] for k, v in a.items()}
    return {json.dumps(expr, sort_keys=True): 1}


def rebuild(terms):
    parts = []
    for key, coefficient in sorted(terms.items()):
        if key == '1':
            parts.append({'const': coefficient})
        else:
            atom = json.loads(key)
            parts.append(atom if coefficient == 1 else {'op': '*', 'args': [atom, {'const': coefficient}]})
    if not parts:
        return {'const': 0}
    result = parts[0]
    for p in parts[1:]:
        result = {'op': '+', 'args': [result, p]}
    return result


def resolve_outer(ctx, var, type_name):
    """An object that a nested loader reads beside its own: the caller's object, an enclosing loop
    element or pointer target of that type, or a parameter of the generated function."""
    wanted = plain(type_name)
    if var == functions[ctx.fid].get('var') or plain(ctx.self_type) == wanted:
        return ('data[0]' if ctx.self_is_array else 'data'), canonical(ctx.self_type)
    for expr, object_type in reversed(ctx.objects):
        if plain(object_type) == wanted:
            return expr, canonical(object_type)
    if var not in ctx.required:
        ctx.required[var] = ('outer_' + ident(type_name).lower(), canonical(type_name))
    return ctx.required[var]


def call_arguments(ctx, fid):
    args = []
    for var, type_name in required.get(fid, []):
        args.append(resolve_outer(ctx, var, type_name)[0])
    return ''.join(', ' + a for a in args)


def substitute_count(expr, count):
    """A callee's self-load size with its count parameter replaced by the caller's count."""
    if expr is None:
        return None
    if 'param' in expr and expr['param'] == 'count':
        return count
    if 'args' in expr:
        return dict(expr, args=[substitute_count(a, count) for a in expr['args']])
    return expr


def self_size(fid, count):
    """C++ byte count of what a loader reads for its own object, or None when it reads none."""
    size = functions[fid].get('self')
    if size is None:
        return None
    return substitute_count(size, count)


def emit_load(ctx, op, out):
    fid = op.get('fn')
    if fid is None:
        raise Unsupported('load of an untyped callee %s' % op['name'])
    f = form(fid)
    at_start = op['at_start'].get('const')
    if at_start == 0:
        # Inline data: the callee's bytes follow the caller's header in the stream (the native loader
        # reads them at the cursor, which is the member's address).
        size = self_size(fid, op['count'])
        if size is None:
            raise Unsupported('inline load without a size: %s' % op['name'])
        if size != {'const': 0}:
            ctx.line('stream.bytes(%s);' % expression(ctx, size), out)
        if f in ('empty',):
            return
        op = dict(op, at_start={'const': 1})
    elif at_start != 1:
        raise Unsupported('load at start outside a pointer: %s' % op['name'])
    want = functions[fid]['type']
    if f in ('array', 'script_strings', 'strings'):
        expr, type_name = access(ctx, op['target'])
        found = descend(expr, type_name, lambda t: is_type(want)(t) or (
            array_element(t)[0] is not None and is_type(want)(array_element(t)[0])))
        if found is None:
            raise Unsupported('array load %s on %s' % (op['name'], type_name))
        expr, type_name = found
        if array_element(type_name)[0] is not None:
            expr = '%s[0]' % expr
    else:
        expr, type_name = access_as(ctx, op['target'], is_type(want))
    if f == 'string':
        ctx.line('stream.string(%s);' % expr, out)
    elif f == 'strings':
        emit_string_loop(ctx, '&%s' % expr, expression(ctx, op['count']), out)
    elif f == 'script_string':
        ctx.line('stream.script_string(%s);' % expr, out)
    elif f == 'script_strings':
        ctx.line('stream.script_strings(&%s, %s);' % (expr, expression(ctx, op['count'])), out)
    elif f == 'asset':
        ctx.line('stream.asset(%s, %s);' % (asset_enum(asset_type_of(fid)), expr), out)
    elif f == 'struct':
        ctx.line('%s(stream, %s%s);' % (function_name(fid), expr, call_arguments(ctx, fid)), out)
    elif f == 'array':
        ctx.line('%s(stream, &%s, %s%s);' % (function_name(fid), expr, expression(ctx, op['count']),
                                             call_arguments(ctx, fid)), out)


def emit_string_loop(ctx, items, count, out):
    index = 'string_%d' % len(out)
    ctx.line('for (std::size_t %s = 0; %s < %s; %s++)' % (index, index, count, index), out)
    ctx.line('{', out)
    ctx.line('\tstream.string((%s)[%s]);' % (items, index), out)
    ctx.line('}', out)


def union_arm(type_name, element, size, size_expr=None):
    """The pointer arm of a union whose target matches `element`, its size, or (for a variable data size)
    the largest target size that divides the byte count."""
    arms = [m for m in members(type_name) if (entry(m['type']) or {}).get('kind') == 'pointer']
    for m in arms:
        if element and pointer_target(m['type']) == element:
            return m
    for m in arms:
        if size and size_of(pointer_target(m['type'])) == size:
            return m
    if size_expr is not None:
        dividing = []
        for m in arms:
            target_size = size_of(pointer_target(m['type']))
            try:
                if target_size:
                    divide(size_expr, target_size)
                    dividing.append((target_size, m))
            except Unsupported:
                pass
        if dividing:
            return max(dividing, key=lambda d: d[0])[1]
    if len(arms) == 1:
        return arms[0]
    raise Unsupported('no union arm of %s for %s' % (type_name, element))


def emit_pointer(ctx, op, out):
    body = list(op['body'])
    # Temp reads and counting loops between the pointer and its data (the element count) come first.
    while body and (body[0]['op'] == 'temp' or (body[0]['op'] == 'loop' and not body[0]['body'])):
        emit_ops(ctx, [body[0]], out)
        body = body[1:]
    # if (c) {} else { data n }: the pointee holds n bytes or none.
    if body and body[0]['op'] == 'if':
        first_if = body[0]
        then_ops, else_ops = first_if['then'], first_if['else']
        if len(then_ops) == 1 and len(else_ops) == 1 and then_ops[0]['op'] == 'data' and \
                else_ops[0]['op'] == 'data' and then_ops[0]['target'] == else_ops[0]['target']:
            data = dict(then_ops[0], size={'select': first_if['cond'], 'then': then_ops[0]['size'],
                                           'else': else_ops[0]['size']})
            body = [data] + body[1:]
        elif not then_ops and else_ops and else_ops[0]['op'] == 'data':
            data = dict(else_ops[0], size={'select': first_if['cond'], 'then': {'const': 0}, 'else': else_ops[0]['size']})
            body = [data] + else_ops[1:] + body[1:]
        elif not else_ops and then_ops and then_ops[0]['op'] == 'data':
            data = dict(then_ops[0], size={'select': first_if['cond'], 'then': then_ops[0]['size'], 'else': {'const': 0}})
            body = [data] + then_ops[1:] + body[1:]
    expr, type_name = access(ctx, op['member'])
    found = descend(expr, type_name, is_kind('pointer', 'union'))
    if found is not None:
        expr, type_name = found
    kind = (entry(type_name) or {}).get('kind')
    first = body[0] if body else None
    element = None
    if first and first['op'] == 'load' and first.get('type'):
        element = canonical(S['split_cv'](first['type'])[1])
    if kind == 'union':
        size = None
        size_expr = None
        if first and first['op'] == 'data':
            size_expr = first['size']
            if 'const' in first['size']:
                size = first['size']['const']
        arm = union_arm(type_name, element, size, size_expr)
        expr, type_name = '%s.%s' % (expr, arm['cxx']), canonical(arm['type'])
        kind = 'pointer'
    if kind != 'pointer':
        raise Unsupported('pointer op on %s (%s)' % (expr, type_name))
    target = pointer_target(type_name)
    align = op['align']['const'] if op['align'] and 'const' in op['align'] else 0
    # An asset pointer loader inlined into its caller: the pointee begins an asset of a known type.
    if -3 in op['markers']:
        begins = [o for o in walk(body) if o['op'] == 'asset_begin' and 'const' in o['type']]
        if not begins and first and first['op'] == 'load' and first.get('fn'):
            # The pointee loader of an inserted pointer begins an asset: the pointer is an asset pointer.
            begins = [o for o in functions[first['fn']]['ops'] if o['op'] == 'asset_begin' and 'const' in o['type']]
        if begins:
            ctx.line('stream.asset(%s, %s);' % (asset_enum(begins[0]['type']['const']), expr), out)
            return
    if first is None:
        # No pointee data (a null-checked pointer whose target loads nothing).
        ctx.line('stream.single(%s, %d);' % (expr, align), out)
        return
    # A pointer to an array is spelled as a pointer to its element in the structs: count in elements.
    element_of_target, _ = array_element(target)
    if element_of_target is not None:
        target = element_of_target
    tsize = size_of(target)
    # read: the stream call that reads the pointee (array of the target type, or raw bytes).
    if first['op'] == 'load':
        fid = first.get('fn')
        if fid is None:
            raise Unsupported('pointer load of an untyped callee')
        f = form(fid)
        if f in ('array', 'script_strings', 'strings') or (f == 'empty' and is_count_size(functions[fid].get('self'))):
            count = expression(ctx, first['count'])
        else:
            count = '1'
        native = functions[fid].get('self')
        terms = linear(native) if native else None
        per_element = None
        if terms is not None:
            per_element = terms.get(json.dumps({'param': 'count'}, sort_keys=True)) if count != '1' else terms.get('1')
        if per_element and tsize and per_element != tsize:
            # The loader's element differs from the pointer target: a header shorter than the record (the
            # rest follows inline), or a group of targets per element (float4[2] behind float4*).
            read = 'reinterpret_cast<%s*>(stream.elements(%s, %d, %d, %s))' % (
                cxx_type(target), expr, align, per_element, count)
        else:
            read = 'stream.array(%s, %d, %s)' % (expr, align, count)
        rest = body[1:]
    elif first['op'] == 'data':
        if not tsize:
            raise Unsupported('pointer to %s of unknown size' % target)
        try:
            count = expression(ctx, divide(first['size'], tsize))
            read = 'stream.array(%s, %d, %s)' % (expr, align, count)
        except Unsupported:
            # A byte count that is not a whole number of elements: read the bytes.
            count = None
            read = 'reinterpret_cast<%s*>(stream.elements(%s, %d, 1, %s))' % (
                cxx_type(target), expr, align, expression(ctx, first['size']))
        fid, f = None, None
        rest = body[1:]
    else:
        raise Unsupported('pointer body starts with %s' % first['op'])
    variable = 'items_%d' % len(out)
    # The values the read needs come first; an unused pointee drops only the lines after them.
    ctx.flush(out)
    start = len(out)
    ctx.line('if (const auto %s = %s)' % (variable, read), out)
    ctx.line('{', out)
    first_inner = len(out)
    ctx.indent += 1
    saved = ctx.item
    ctx.item = (variable, target, True)
    ctx.objects.append(('%s[0]' % variable, target))
    if f == 'struct':
        ctx.line('%s(stream, *%s%s);' % (function_name(fid), variable, call_arguments(ctx, fid)), out)
    elif f == 'array':
        ctx.line('%s(stream, %s, %s%s);' % (function_name(fid), variable, count, call_arguments(ctx, fid)), out)
    elif f == 'script_strings':
        ctx.line('stream.script_strings(%s, %s);' % (variable, count), out)
    elif f == 'script_string':
        ctx.line('stream.script_string(*%s);' % variable, out)
    elif f == 'string':
        ctx.line('stream.string(*%s);' % variable, out)
    elif f == 'strings':
        emit_string_loop(ctx, variable, count, out)
    elif f == 'asset':
        ctx.line('stream.asset(%s, *%s);' % (asset_enum(asset_type_of(fid)), variable), out)
    emit_ops(ctx, rest, out)
    ctx.objects.pop()
    ctx.item = saved
    ctx.indent -= 1
    if len(out) == first_inner:
        # Nothing reads through the items: read the pointee alone.
        del out[start:]
        ctx.pending = []
        ctx.line('%s;' % read, out)
        return
    ctx.line('}', out)


def emit_loop(ctx, op, out):
    loop = op['id']
    if op['count'] is None:
        raise Unsupported('loop without a count')
    body = [o for o in op['body'] if o['op'] not in ('data', 'hook', 'register', 'store')]
    # A loop that reads nothing only defines the count of a later sum over it.
    ctx.loop_counts[loop] = op['count']
    if not body:
        return
    count = expression(ctx, op['count'])
    index = 'i%d' % loop
    ctx.loops[loop] = index
    pushed = 0
    for element in op['elements']:
        try:
            expr, type_name = access(ctx, dict(element['start'], path=list(element['start'].get('path', [])) +
                                               [{'index': loop, 'stride': element['stride']}]))
        except Unsupported:
            continue
        ctx.objects.append((expr, type_name))
        pushed += 1
    ctx.line('for (std::size_t %s = 0; %s < %s; %s++)' % (index, index, count, index), out)
    ctx.line('{', out)
    ctx.indent += 1
    emit_ops(ctx, op['body'], out)
    ctx.indent -= 1
    ctx.line('}', out)
    del ctx.objects[len(ctx.objects) - pushed:]
    del ctx.loops[loop]


def emit_ops(ctx, ops, out):
    for op in ops:
        kind = op['op']
        if kind in ('data', 'hook', 'register', 'store', 'string_data'):
            continue
        if kind == 'script_string_data':
            expr, _ = access_as(ctx, op['member'], is_type('scr_string_t'))
            ctx.line('stream.script_string(%s);' % expr, out)
            continue
        if kind == 'align':
            mask = op['mask'].get('const') if isinstance(op['mask'], dict) else op['mask']
            if mask:
                ctx.line('stream.align(%d);' % mask, out)
        elif kind == 'push':
            ctx.line('stream.push(%s);' % expression(ctx, op['block']), out)
        elif kind == 'pop':
            ctx.line('stream.pop();', out)
        elif kind == 'asset_begin':
            ctx.line('stream.begin_asset();', out)
        elif kind == 'asset_end':
            ctx.line('stream.end_asset();', out)
        elif kind == 'load':
            emit_load(ctx, op, out)
        elif kind == 'pointer':
            emit_pointer(ctx, op, out)
        elif kind == 'loop':
            emit_loop(ctx, op, out)
        elif kind == 'if':
            then_out, else_out = [], []
            ctx.indent += 1
            emit_ops(ctx, op['then'], then_out)
            emit_ops(ctx, op['else'], else_out)
            ctx.indent -= 1
            if not then_out and not else_out:
                continue
            if not then_out:
                ctx.line('if (%s)' % condition(ctx, op['cond'], negate=True), out)
                ctx.line('{', out)
                out.extend(else_out)
                ctx.line('}', out)
                continue
            ctx.line('if (%s)' % condition(ctx, op['cond']), out)
            ctx.line('{', out)
            out.extend(then_out)
            ctx.line('}', out)
            if else_out:
                ctx.line('else', out)
                ctx.line('{', out)
                out.extend(else_out)
                ctx.line('}', out)
        elif kind == 'switch':
            ctx.line('switch (%s)' % expression(ctx, op['selector']), out)
            ctx.line('{', out)
            for case in op['cases']:
                for value in case['values']:
                    ctx.line('case %d:' % value, out)
                ctx.indent += 1
                emit_ops(ctx, case['ops'], out)
                ctx.line('break;', out)
                ctx.indent -= 1
            ctx.line('}', out)
        elif kind == 'inline':
            emit_ops(ctx, op['ops'], out)
        elif kind == 'temp':
            size = op['size'].get('const')
            spelled = WIDTH_TYPES.get((size, False))
            if spelled is None:
                raise Unsupported('temp of %s bytes' % size)
            name = 'temp_%d' % op['id']
            alignment = (op.get('alignment') or {}).get('const', size)
            value = TEMP_VALUES.get((ctx.fid, op['id']))
            written = 'stream.template missing_temp<%s>()' % spelled if value is None else value(ctx)
            ctx.line('const auto %s = stream.template temp<%s>(%d, [&]() -> %s { return %s; });' % (
                name, spelled, max(alignment - 1, 0), spelled, written), out)
            ctx.temps[op['id']] = name
        else:
            raise Unsupported('op %s' % kind)


required = {}  # fid -> [(var, type)] outer objects the caller passes, in parameter order


def cell_tree_count(ctx):
    # Load_GfxWorldDrawCells reads aabbTreeCounts before aabbTrees: the count of cell tree i is entry i.
    outer, _ = resolve_outer(ctx, CELLS_VAR, 'GfxWorldDrawCells')
    return '%s.aabbTreeCounts[&data - %s.aabbTrees].aabbTreeCount' % (outer, outer)


CELLS_VAR = next(f['var'] for f in functions.values() if f['name'] == 'Load_GfxWorldDrawCells')
TEMP_VALUES = {('Load_GfxCellTree@GfxCellTree', 3): cell_tree_count}


def emit_signature(fid):
    f = functions[fid]
    name = function_name(fid)
    type_name = canonical(S['split_cv'](f['type'])[1])
    params = ['const %s& %s' % (ident(t), 'outer_' + ident(t).lower()) for _, t in required.get(fid, [])]
    extra = ''.join(', [[maybe_unused]] ' + p for p in params)
    if form(fid) == 'array':
        return 'template <typename Stream>\n\t\tvoid %s(Stream& stream, %s* data, const std::size_t count%s)' % (
            name, cxx_type(type_name), extra)
    return 'template <typename Stream>\n\t\tvoid %s(Stream& stream, %s& data%s)' % (name, cxx_type(type_name), extra)


def emit_function(fid):
    f = functions[fid]
    type_name = canonical(S['split_cv'](f['type'])[1])
    skip_zero = False
    element, _ = array_element(type_name)
    if element is not None:
        type_name, skip_zero = element, True
    ctx = Context(fid, 'data', type_name, form(fid) == 'array', {})
    ctx.skip_zero = skip_zero
    body = []
    try:
        emit_ops(ctx, f['ops'], body)
    finally:
        required[fid] = [(var, t) for var, (_, t) in ctx.required.items()]
    return emit_signature(fid), body


# ---- asset type enum --------------------------------------------------------------------------
ASSET_ENUM = {}
POOL_NAMES = {
    'physicslibrary': 'PHYSICSLIBRARY', 'physicssfxeventasset': 'PHYSICS_SFX_EVENT_ASSET',
    'physicsvfxeventasset': 'PHYSICS_VFX_EVENT_ASSET', 'physicsasset': 'PHYSICSASSET',
    'physicsfxpipeline': 'PHYSICS_FX_PIPELINE', 'physicsfxshape': 'PHYSICS_FX_SHAPE',
    'physicsdebugdata': 'PHYSICS_DEBUG_DATA', 'xanim': 'XANIMPARTS', 'xmodelsurfs': 'XMODEL_SURFS', 'xmodel': 'XMODEL',
    'mayhem': 'MAYHEM', 'material': 'MATERIAL', 'computeshader': 'COMPUTESHADER', 'libshader': 'LIBSHADER',
    'vertexshader': 'VERTEXSHADER', 'hullshader': 'HULLSHADER', 'domainshader': 'DOMAINSHADER',
    'pixelshader': 'PIXELSHADER', 'techset': 'TECHNIQUE_SET', 'image': 'IMAGE', 'soundglobals': 'SOUND_GLOBALS',
    'soundbank': 'SOUND_BANK', 'soundbanktransient': 'SOUND_BANK_TRANSIENT', 'col_map': 'CLIPMAP',
    'com_map': 'COMWORLD', 'glass_map': 'GLASSWORLD', 'aipaths': 'PATHDATA', 'navmesh': 'NAVMESH',
    'tacgraph': 'TACGRAPH', 'map_ents': 'MAP_ENTS', 'fx_map': 'FXWORLD', 'gfx_map': 'GFXWORLD',
    'gfx_map_trzone': 'GFXWORLD_TRANSIENT_ZONE', 'iesprofile': 'IESPROFILE', 'lightdef': 'LIGHT_DEF',
    'gradingclut': 'GRADING_CLUT', 'ui_map': 'UI_MAP', 'fogspline': 'FOG_SPLINE', 'animclass': 'ANIMCLASS',
    'playeranim': 'PLAYERANIM', 'gesture': 'GESTURE', 'localize': 'LOCALIZE_ENTRY', 'attachment': 'ATTACHMENT',
    'weapon': 'WEAPON', 'vfx': 'VFX', 'impactfx': 'IMPACT_FX', 'surfacefx': 'SURFACE_FX', 'aitype': 'AITYPE',
    'mptype': 'MPTYPE', 'character': 'CHARACTER', 'xmodelalias': 'XMODELALIAS', 'rawfile': 'RAWFILE',
    'scriptfile': 'SCRIPTFILE', 'scriptdebugdata': 'SCRIPT_DEBUG_DATA', 'stringtable': 'STRINGTABLE',
    'leaderboarddef': 'LEADERBOARD', 'virtualleaderboarddef': 'VIRTUAL_LEADERBOARD', 'ddl': 'DDL',
    'tracer': 'TRACER', 'vehicle': 'VEHICLE', 'addon_map_ents': 'ADDON_MAP_ENTS',
    'netconststrings': 'NET_CONST_STRINGS', 'luafile': 'LUA_FILE', 'scriptable': 'SCRIPTABLE',
    'equipsndtable': 'EQUIPMENT_SND_TABLE', 'vectorfield': 'VECTORFIELD',
    'particlesimanimation': 'PARTICLE_SIM_ANIMATION', 'streaminginfo': 'STREAMING_INFO', 'laser': 'LASER',
    'ttf': 'TTF', 'suit': 'SUIT', 'suitanimpackage': 'SUITANIMPACKAGE', 'camera': 'CAMERA',
    'hudoutline': 'HUDOUTLINE', 'spaceshiptarget': 'SPACESHIPTARGET', 'rumble': 'RUMBLE',
    'rumblegraph': 'RUMBLE_GRAPH', 'animpkg': 'ANIM_PACKAGE', 'sfxpkg': 'SFX_PACKAGE', 'vfxpkg': 'VFX_PACKAGE',
    'footstepvfx': 'FOOTSTEP_VFX', 'behaviortree': 'BEHAVIOR_TREE', 'aianimset': 'ANIMSET', 'aiasm': 'ASM',
    'proceduralbones': 'XANIM_PROCEDURALBONES', 'dynamicbones': 'XANIM_DYNAMICBONES', 'reticle': 'RETICLE',
    'xanimcurve': 'XANIMCURVE', 'coverselector': 'COVERSELECTOR', 'enemyselector': 'ENEMYSELECTOR',
    'clientcharacter': 'CLIENTCHARACTER', 'clothasset': 'CLOTHASSET', 'cinematicmotion': 'CINEMATICMOTION',
    'accessory': 'ACCESSORY', 'locdmgtable': 'LOCDMGTABLE', 'bulletpenetration': 'BULLETPENETRATION',
    'scriptbundle': 'SCRIPTBUNDLE', 'blendspace2d': 'BLENDSPACE2D', 'xcam': 'XCAM', 'camo': 'CAMO',
    'xcompositemodel': 'XCOMPOSITEMODEL', 'xmodeldetailcollision': 'XMODEL_DETAIL_COLLISION',
    'streamkey': 'STREAM_KEY', 'streamtreeoverride': 'STREAM_TREE_OVERRIDE', 'keyvaluepairs': 'KEYVALUEPAIRS',
    'stterrain': 'SUPER_TERRAIN', 'nativescriptpatch': 'NATIVE_SCRIPT_PATCH', 'collisiontile': 'COLLISION_TILE',
    'execution': 'EXECUTION', 'carryobject': 'CARRYOBJECT', 'soundbanklist': 'SOUNDBANKLIST',
    'decalvolumematerial': 'DECAL_VOLUME_MATERIAL', 'decalvolumemask': 'DECAL_VOLUME_MASK',
    'dynentitylist': 'DYNENTITY_LIST', 'fx_map_trzone': 'FXWORLD_TRANSIENT_ZONE', 'dlogschema': 'DLOG_SCHEMA',
    'edgelist': 'EDGE_LIST',
}
for pool in S['profile']['pools']:
    ASSET_ENUM[pool['id']] = 'ASSET_TYPE_' + POOL_NAMES[pool['name']]


# ---- output -------------------------------------------------------------------------------------
# The stream classes of zone/streams.hpp that the asset table instantiates.
STREAMS = ['load_stream', 'write_stream']


def main():
    emitted = {}
    failures = collections.OrderedDict()
    order = []

    in_progress = set()

    def need(fid):
        if fid in emitted or fid in failures or fid in in_progress:
            return
        if form(fid) not in ('struct', 'array'):
            return
        # Callees first: a caller passes the outer objects its callees need.
        in_progress.add(fid)
        for op in walk(functions[fid]['ops']):
            if op['op'] == 'load' and op.get('fn'):
                need(op['fn'])
        in_progress.discard(fid)
        try:
            signature, body = emit_function(fid)
        except Unsupported as e:
            # The callers stay valid: the reader stops with a clear error when a zone reaches it.
            failures[fid] = str(e)
            signature = emit_signature(fid)
            message = '%s: %s' % (functions[fid]['name'], str(e).replace('\\', '/').replace('"', "'"))
            body = ['\t\tstream.unsupported("%s");' % message]
        emitted[fid] = (signature, body)
        for op in walk(functions[fid]['ops']):
            if op['op'] == 'load' and op.get('fn'):
                need(op['fn'])
        order.append(fid)

    roots = []
    for asset in spec['assets']:
        need(asset['loader'])
        roots.append(asset)

    # Several loaders differ only by the asset type they register (the shader pools): keep one body.
    alias = {}
    while True:
        seen = {}
        changed = False
        for fid in order:
            if fid in alias:
                continue
            signature, body = emitted[fid]
            name = function_name(fid)
            key = (signature.replace(name + '(', '@('), tuple(body))
            if key in seen:
                alias[fid] = seen[key]
                changed = True
            else:
                seen[key] = fid
        if not changed:
            break
        names = {function_name(fid): function_name(target) for fid, target in alias.items()}
        pattern = re.compile(r'\b(%s)\(' % '|'.join(re.escape(n) for n in sorted(names, key=len, reverse=True)))
        for fid in order:
            signature, body = emitted[fid]
            emitted[fid] = (signature, [pattern.sub(lambda m: names[m[1]] + '(', line) for line in body])
    order = [fid for fid in order if fid not in alias]

    def entry_name(fid):
        while fid in alias:
            fid = alias[fid]
        return function_name(fid)

    hpp = ['#pragma once', '',
           '// Generated by tools/gen_iw8_streams.py from tools/iw8_format.json (the Replay 1.20 Load_* routines).',
           '', 'namespace zonetool::iw8', '{', '\tenum XAssetType', '\t{']
    for pool in S['profile']['pools']:
        hpp.append('\t\t%s = %d,' % (ASSET_ENUM[pool['id']], pool['id']))
    hpp += ['\t\tASSET_TYPE_COUNT = %d,' % len(S['profile']['pools']), '\t};', '']
    hpp += ['\tstruct asset_format', '\t{', '\t\tconst char* name;', '\t\tstd::size_t size;',
            '\t\tstd::size_t alignment;',
            '\t\t// The block that the map preload (Preload_*Ptr) loads the header to: 1, or 2 that does not rewind.',
            '\t\tstd::uint8_t preload_block;']
    for stream in STREAMS:
        hpp.append('\t\tvoid (*%s)(%s& stream, void* asset);' % (stream[:-len('_stream')], stream))
    hpp += ['\t};', '',
            '\t// The format of an asset type, or nullptr for a type that zones do not hold.',
            '\tconst asset_format* find_asset_format(std::int32_t type);', '}', '']

    cpp = ['#include <std_include.hpp>', '#include "../iw8.hpp"', '', '// Generated by tools/gen_iw8_streams.py.',
           '', 'namespace zonetool::iw8', '{', '\tnamespace', '\t{']
    for fid in order:
        cpp.append('\t\t%s;' % emitted[fid][0])
    cpp.append('')
    for fid in order:
        signature, body = emitted[fid]
        cpp.append('\t\t' + signature)
        cpp.append('\t\t{')
        cpp.extend(['\t' + line for line in body])
        cpp.append('\t\t}')
        cpp.append('')
    for asset in roots:
        if asset['loader'] in emitted:
            fid = asset['loader']
            cpp += ['\t\ttemplate <typename Stream>',
                    '\t\tvoid stream_%s_asset(Stream& stream, void* asset)' % asset['pool'], '\t\t{',
                    '\t\t\t%s(stream, *static_cast<%s*>(asset));' % (entry_name(fid), ident(asset['type'])),
                    '\t\t}', '']
    cpp.append('\t}')
    cpp.append('')

    # Asset table: header size, pointer alignment and the streams of every pool.
    by_id = {asset['id']: asset for asset in roots if asset['loader'] in emitted}
    cpp += ['\tconst asset_format* find_asset_format(const std::int32_t type)', '\t{',
            '\t\tstatic const asset_format formats[ASSET_TYPE_COUNT]', '\t\t{']
    for pool in S['profile']['pools']:
        asset = by_id.get(pool['id'])
        if asset is None:
            cpp.append('\t\t\t{"%s", 0, 0, 0%s},' % (pool['name'], ', nullptr' * len(STREAMS)))
            continue
        pointer = functions[asset['pointer_loader']]
        alignment = next((o['align']['const'] for o in pointer['ops'] if o['op'] == 'pointer' and o['align']), 7)
        streams = ''.join(', stream_%s_asset<%s>' % (asset['pool'], stream) for stream in STREAMS)
        cpp.append('\t\t\t{"%s", %d, %d, %d%s},' % (pool['name'], pool['size'], alignment,
                                                    asset.get('preload_block', 1), streams))
    cpp += ['\t\t};', '']
    for pool in S['profile']['pools']:
        asset = by_id.get(pool['id'])
        if asset is not None:
            cpp.append('\t\tstatic_assert(sizeof(%s) == %d);' % (ident(asset['type']), pool['size']))
    cpp += ['',
            '\t\tif (type < 0 || type >= ASSET_TYPE_COUNT || !formats[type].load)', '\t\t{', '\t\t\treturn nullptr;',
            '\t\t}', '', '\t\treturn &formats[type];', '\t}', '']
    cpp += ['}', '']

    os.makedirs(OUT, exist_ok=True)
    open(os.path.join(OUT, 'asset_streams.hpp'), 'w', encoding='utf-8', newline='\r\n').write('\n'.join(hpp))
    open(os.path.join(OUT, 'asset_streams.cpp'), 'w', encoding='utf-8', newline='\r\n').write('\n'.join(cpp))
    print('emitted %d functions for %d of %d assets; %d failed' % (
        len(emitted), sum(1 for a in roots if a['loader'] in emitted), len(roots), len(failures)))
    for fid, why in failures.items():
        print('  %s: %s' % (fid, why))


main()

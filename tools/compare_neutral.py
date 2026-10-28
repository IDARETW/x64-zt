"""compare_neutral.py <folder a> <folder b>: compares the neutral data files that two adapters wrote (NEUTRAL_OUT).

Each <kind>/<name>.json present in both folders must hold equal data; files present in only one are listed.
Exit 1 when a common file differs.
"""
import json
import sys
from pathlib import Path


def first_difference(a, b, path='$'):
    if type(a) is not type(b):
        return f'{path}: {type(a).__name__} / {type(b).__name__}'
    if isinstance(a, dict):
        for key in sorted(set(a) | set(b)):
            if key not in a or key not in b:
                return f'{path}.{key}: present in one'
            found = first_difference(a[key], b[key], f'{path}.{key}')
            if found:
                return found
        return None
    if isinstance(a, list):
        if len(a) != len(b):
            return f'{path}: {len(a)} / {len(b)} items'
        for index, (x, y) in enumerate(zip(a, b)):
            found = first_difference(x, y, f'{path}[{index}]')
            if found:
                return found
        return None
    return None if a == b else f'{path}: {a!r} / {b!r}'


def main():
    a, b = Path(sys.argv[1]), Path(sys.argv[2])
    files_a = {p.relative_to(a) for p in a.rglob('*.json')}
    files_b = {p.relative_to(b) for p in b.rglob('*.json')}
    common = sorted(files_a & files_b)
    different = 0
    for name in common:
        found = first_difference(json.loads((a / name).read_text()), json.loads((b / name).read_text()))
        if found:
            different += 1
            print(f'differs {name}: {found}')
    print(f'{len(common)} common, {len(common) - different} equal, {different} different; '
          f'{len(files_a - files_b)} only in {a}, {len(files_b - files_a)} only in {b}')
    for name in sorted(files_a - files_b)[:20]:
        print(f'only in a: {name}')
    for name in sorted(files_b - files_a)[:20]:
        print(f'only in b: {name}')
    return 1 if different else 0


if __name__ == '__main__':
    sys.exit(main())

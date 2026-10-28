"""compare_dump_trees.py <tree a> <tree b>: compares two dumpzone folders file by file and counts differences by asset type.

The record-level gate of docs/NEUTRAL_ASSETS.md: dumpzone a stock zone and the same zone rebuilt from neutral data
(IW8_REWRITE=neutral with IW8_REWRITE_OUT) and compare the folders. Exit 1 when a file differs or a name is on one side.
"""
import collections
import hashlib
import os
import sys


def tree(root):
    out = {}
    for directory, _, files in os.walk(root):
        for name in files:
            path = os.path.join(directory, name)
            with open(path, 'rb') as handle:
                out[os.path.relpath(path, root).replace(os.sep, '/')] = hashlib.sha256(handle.read()).hexdigest()
    return out


a, b = tree(sys.argv[1]), tree(sys.argv[2])
print('files', len(a), len(b), 'same names', a.keys() == b.keys())
total = collections.Counter()
different = collections.Counter()
examples = {}
for name in sorted(a):
    parts = name.split('/')
    kind = parts[2] if len(parts) > 3 else parts[0]
    total[kind] += 1
    if name not in b or a[name] != b[name]:
        different[kind] += 1
        examples.setdefault(kind, name)
print('different files by type:', dict(different) or 'none')
print('examples:', examples)
print('files by type:', dict(total.most_common(14)))
sys.exit(0 if a == b else 1)

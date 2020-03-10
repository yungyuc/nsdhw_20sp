#!/usr/bin/env python3

import os, sys

executable = os.environ['PYTHON_BIN']

code = """
import sys
import os.path

# print(sys.version)

if len(sys.argv) < 2:
    sys.stdout.write('missing file name\\n')
elif len(sys.argv) > 2:
    sys.stdout.write('only one argument is allowed\\n')
else:
    fname = sys.argv[1]
    if os.path.exists(fname):
        with open(fname) as fobj:
            lines = fobj.readlines()
        sys.stdout.write('{} lines in {}\\n'.format(len(lines), fname))
    else:
        sys.stdout.write('{} not found\\n'.format(fname))
 """

filepath = sys.argv[1] if len(sys.argv) > 1 else ''
cmd = 'exec {} -c \"{}\" {}'.format(executable, code, filepath)

os.execlp('bash', 'bash', '-c', cmd)

#!/usr/bin/env python3
import sys
import os
import subprocess
ver=os.environ['PYTHON_BIN']

if ver== 'python2':
    cmd = 'PYTHON_BIN=python3 python2 ./countline.py ' + "".join(sys.argv[1:])
    retcode = subprocess.call(cmd, shell=True)
    if retcode != 0: sys.exit(retcode)

elif ver == 'python3':
    if len(sys.argv) < 2:
        sys.stdout.write('missing file name\n')
    elif len(sys.argv) > 2:
        sys.stdout.write('only one argument is allowed\n')
    else:
        fname = sys.argv[1]
        if os.path.exists(fname):
            with open(fname) as fobj:
                lines = fobj.readlines()
            sys.stdout.write('{} lines in {}\n'.format(len(lines), fname))
        else:
            sys.stdout.write('{} not found\n'.format(fname))

else:
    print("exec: {}: not found".format(ver))
    sys.exit(1)

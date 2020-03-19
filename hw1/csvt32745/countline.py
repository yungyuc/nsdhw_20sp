#!/usr/bin/env python

import sys
import os
import subprocess

pyEnv = os.environ.get('PYTHON_BIN', " ")

if pyEnv == "python2":
    # execute python2
    if sys.version_info.major == 2:
        pass
    else:
        #print("{0} to {1}".format(sys.version_info.major, 2))
        subprocess.call(["python2", sys.argv[0]])
        exit(0)
elif pyEnv == "python3":
    # execute python3
    if sys.version_info.major == 3:
        pass
    else:
        #print("{0} to {1}".format(sys.version_info.major, 3))
        subprocess.call(["python3"] + sys.argv[0:2])
        exit(0)
else:
    sys.stdout.write("exec: {0}: not found\n".format(pyEnv))
    exit(1)
    
# Main Procedural

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

#!/bin/bash
# shell command
""":"
# check the environment variable PYTHON_BIN to use the proper Python executable
pybin=$PYTHON_BIN

#if [[ -x $pybin ]]; then
if [[ -x "$(command -v $pybin)" ]]; then
  exec $pybin $0 "$@"
else
  exit 127
fi
":"""
# Python

import sys
import os.path


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

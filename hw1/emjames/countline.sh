#!/usr/bin/sh

if [[ $# -lt 1 ]]; then
  echo $'missing file name \n'
elif [[ $# -gt 1 ]]; then
  echo $'only one argument is allowed \n'
elif [[ ! -e $1 ]]; then
  echo $'$1 not found \n'
else
  lines=$(grep -c ^ $1)
  echo $'$lines lines in $1 \n'
fi


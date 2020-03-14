#!/bin/bash

if [[ $# -lt 1 ]]; then
  echo $'missing file name \n'
elif [[ $# -gt 1 ]]; then
  echo $'only one argument is allowed \n'
elif [[ ! -e $1 ]]; then
  printf "%s not found \n" "$1"
else
  lines=$(grep -c ^ $1)
  printf "%s lines in %s \n" "$lines" "$1"  
fi


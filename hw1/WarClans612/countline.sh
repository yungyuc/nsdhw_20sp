#!/bin/bash
# Developer: Wilbert (wilbert.phen@gmail.com)

argc=$#
if [ $argc -lt 1 ]
then
    echo "missing file name"
elif [ $argc -gt 1 ]
then
    echo "only one argument is allowed"
else
    fname=$1
    if [ -f "$fname" ]
    then
        echo "$(grep -c "" "$fname") lines in $fname"
    else
        echo "$fname not found"
    fi
fi

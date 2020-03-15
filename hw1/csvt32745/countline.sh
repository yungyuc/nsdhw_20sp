#!/bin/bash

if `test $# -lt 1`; then
    echo "missing file name"
elif `test $# -gt 1`; then
    echo "only one argument is allowed"
else
    fname=$1
    if `test -e $fname`; then
        count=0
        while read -r > /dev/null
        do
            let count+=1
        done < $fname
        let count+=1
        echo "$count lines in $fname"
    else
        echo "$fname not found"
    fi
fi
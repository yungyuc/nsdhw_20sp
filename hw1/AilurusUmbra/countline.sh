#!/bin/bash
if [[ $# -lt 1 ]]; then
		echo "missing file name"    
elif [[ $# -gt 1 ]]; then
		echo "only one argument is allowed"
else
    [[ -f $1 ]] && echo $(wc -l $1 |  awk -F " " '{print $1,"lines in",$2}') || echo $1 "not found"
fi

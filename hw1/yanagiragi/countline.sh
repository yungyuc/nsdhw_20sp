#!/bin/bash

argc=$#
filepath=$1

if [ $argc -lt 1 ]; then
	echo "missing file name"
elif [ $argc -gt 1 ]; then
	echo "only one argument is allowed"
else
	if [ -f $filepath ]; then
		echo "$(wc -l $filepath | awk '{print $1}') lines in $filepath"
	else
		echo "$filepath not found"
	fi
fi


#!/bin/bash

file=$1 
arguments=$#

if [ $arguments -eq 0 ] 
then
	echo "missing file name"
elif [ $arguments -gt 1 ] 
then
	echo "only one argument is allowed"
else

	if [ -f $file ]
	then
		echo "$(cat $file | wc -l) lines in $file"
	else
		echo "$file not found"
	fi
fi
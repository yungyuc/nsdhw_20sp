#!/bin/bash

var=$1
nb=$#


if [ $nb -eq 0 ] ; then
	echo "missing file name"
elif [ $nb -gt 1 ] ; then
	echo "only one argument is allowed"
else
	if [ -f $var ] ; then
		echo "$(cat $var | wc -l) lines in $var"
	else
		echo "$var not found"
	fi
fi



#!/bin/bash

if [ $# -lt 1 ] 
then 
   echo "missing file name\n"
elif [ $# -gt 1 ]
then
   echo "only one argument is allowed\n"
else
   wc -l < $1
fi

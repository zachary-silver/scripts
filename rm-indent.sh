#!/bin/bash

## Removes all indentation from each line a file.

if [ $# -eq 0 ] ; then
	echo "File name needed as parameter!"
	exit 1
fi

if [ ! -e "$1" ] ; then
	echo "No such file exists!"
	exit 1
fi

touch "$1.new"

sed -r 's/^[[:blank:]]+//' "$1" > "$1.new"

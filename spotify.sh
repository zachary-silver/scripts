#!/bin/bash

if [[ ! $(pgrep -lf spotifyd) ]]; then
	/usr/bin/spotifyd
fi

st -c spotify -e spt &
exit

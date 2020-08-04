#!/bin/bash

if [[ ! $(pgrep -lf spotifyd) ]]; then
	spotifyd
fi

st -c spotify -e spt &
exit

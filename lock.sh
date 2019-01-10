#!/bin/bash

GREY="#2f3444d1"
BLACK="#1f222dd1"
RED="#ff9554d1"
GREEN="#90db95d1"
YELLOW="#f2ef93d1"
BLUE="#4e8cb7d1"
MAGENTA="#ba5d89d1"
CYAN="#79d1c9d1"
WHITE="#ffffffff"

lock_screen="$HOME/Pictures/lock_screen.png"

if [ ! -e "$lock_screen" ]; then
	scrot "$lock_screen"
	# simpler and faster method but more primitive result
	#convert "$lock_screen" -scale 10% -scale 1000% "$lock_screen"
	convert "$lock_screen" -blur 0x4 "$lock_screen"
fi

i3lock -e -i "$lock_screen" \
--screen=1	      \
--timepos="ix:iy-5"  \
--timecolor=$WHITE    \
--datepos="ix:iy+15"  \
--datestr "Type password to unlock..." \
--datecolor=$WHITE    \
--clock		      \
--force-clock	      \
--noinputtext=""      \
--veriftext=""	      \
--wrongtext=""	      \
--radius=110	      \
--indicator	      \
\
--insidevercolor=$GREY \
--ringvercolor=$YELLOW \
--verifcolor=$WHITE   \
\
--insidewrongcolor=$GREY \
--ringwrongcolor=$RED   \
--wrongcolor=$WHITE   \
\
--insidecolor=$GREY   \
--ringcolor=$BLACK	\
--linecolor=$BLACK    \
--separatorcolor=$BLACK	\
--keyhlcolor=$BLUE	\
--bshlcolor=$RED      \

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

lock_screen="$HOME/pictures/lock_screen.png"

if [ ! -e "$lock_screen" ]; then
	scrot "$lock_screen"
	# simpler and faster method but more primitive result
	#convert "$lock_screen" -scale 10% -scale 1000% "$lock_screen"
	convert "$lock_screen" -blur 0x4 "$lock_screen"
fi

i3lock -e -i "$lock_screen" \
--screen=1	      \
--time-pos="ix:iy+8"  \
--time-color=$WHITE    \
--date-pos="ix:iy+15"  \
--date-str "" \
--date-color=$WHITE    \
--clock		      \
--force-clock	      \
--radius=110	      \
--indicator	      \
\
--insidever-color=$GREY \
--ringver-color=$WHITE \
--verif-color=$GREEN   \
\
--insidewrong-color=$GREY \
--ringwrong-color=$RED   \
--wrong-color=$WHITE   \
\
--inside-color=$GREY   \
--ring-color=$BLACK	\
--line-color=$BLACK    \
--separator-color=$BLACK	\
--keyhl-color=$WHITE	\
--bshl-color=$RED      \
\
--verif-text="" \
--wrong-text="" \
--noinput-text=""

#!/bin/bash

artwork_file="/home/zack/.config/spotifyd/cache/album_artwork"
artwork_url=$(playerctl metadata | grep artUrl | awk '{print $3}')
song=$(playerctl metadata --format "{{ artist }} - {{ title }}")
album=$(playerctl metadata --format "{{ album }}")

# download artwork
curl -wL $artwork_url -o $artwork_file 1> /dev/null 2> /dev/null
# resize image
convert $artwork_file -resize 128x128 $artwork_file
# send notification
notify-send -i $artwork_file "$song" "$album"

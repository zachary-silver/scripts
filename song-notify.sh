#!/bin/bash

artwork_file="/home/zack/.config/spotifyd/cache/album_artwork"
artwork_url=$(playerctl --player=spotifyd metadata | grep artUrl | awk '{print $3}')
song=$(playerctl metadata --player=spotifyd --format "{{ artist }} - {{ title }}")
album=$(playerctl metadata --player=spotifyd --format "{{ album }}")

# download artwork
curl -wL $artwork_url -o $artwork_file 1> /dev/null 2> /dev/null
# resize image
convert $artwork_file -resize 128x128 $artwork_file
# send notification
notify-send -i $artwork_file "$song" "$album"

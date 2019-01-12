#!/bin/bash

# get_cpu_usage() requires mpstat package to work
# get_song() requires jq package to work

get_battery()
{
	if ! pacman -Qi acpi &> /dev/null ; then # acpi not installed / not a laptop
		return 1
	fi

	icon=""
	full=""
	semifull=""
	half=""
	semiempty=""
	empty=""

	bat0=$(acpi -b | awk -F '[,:%]' 'FNR == 1 { printf $3 }')
  	bat1=$(acpi -b | awk -F '[,:%]' 'FNR == 2 { printf $3 }')
	total=$((${bat0} + ${bat1}))
	percent=$(jq -n ${total}/2)
	percent=$(printf %.0f ${percent})

	if [ $percent -ge 90 ]; then
		icon=$full
	elif [ $percent -ge 60 ]; then
		icon=$semifull
	elif [ $percent -ge 30 ]; then
		icon=$half
	elif [ $percent -ge 10 ]; then
		icon=$semiempty
	else
		icon=$empty
	fi

  	echo "${icon} ${percent}%"

	return 0
}

get_cpu_usage()
{
	icon=""
	used="$(mpstat -u | awk 'FNR == 4 { print $4 }')"
	cpu_usage="$(printf %3.1f ${used})%"

	echo "${icon} ${cpu_usage}"

	return 0
}

get_date()
{
	icon=""
	date=$(date '+%a %b/%d/%Y')

	echo "${icon} ${date}"

	return 0
}

get_disk_usage()
{
	icon=""
	disk_usage=$(df -h / | awk 'FNR == 2 { printf("%d/%d", $3, $2) }')
	disk_usage="${disk_usage}G"

	echo "${icon} ${disk_usage}"

	return 0
}

get_memory_usage()
{
	icon=""
	total_memory=$(cat /proc/meminfo | grep MemTotal: | awk '{ printf("%d", ($2+500000)/1000000) }')
	used_memory=$(cat /proc/meminfo | grep Active: | awk '{ printf("%d", ($2+500000)/1000000) }')
	memory_usage="${used_memory}/${total_memory}"
	memory_usage="$(printf %4s ${memory_usage})G"

	echo "${icon} ${memory_usage}"

	return 0
}

get_ip()
{
	icon=""
	interface=$(ip route | awk '/^default/ { print $5 ; exit }')

	if [[ "$(cat /sys/class/net/${interface}/operstate)" = 'down' ]]; then
		echo "down"
	else
		case $1 in
		  -4)
		    af=inet ;;
		  -6)
		    af=inet6 ;;
		  *)
		    af=inet6? ;;
		esac

		# if no interface is found, use the first device with a global scope
		perlcmd="/${af} ([^\/]+).* scope global/ && print \$1 and exit"
		ip=$(ip addr show ${interface} | perl -n -e "${perlcmd}")

		echo "${icon} ${ip}"
	fi

	return 0
}

get_song_data()
{
	data=$1
	gpmdir="$(cat ${HOME}/.config/Google\ Play\ Music\ Desktop\ Player/json_store/playback.json)"

	echo "$(echo ${gpmdir} | jq ${data} | sed 's/"//g')"

	return 0
}

get_song()
{
	paused=""
	playing=""
	stopped=""

	player_status="$(get_song_data '.playing')"
	title="$(get_song_data '.song.title')"
	artits="$(get_song_data '.song.artist')"
	liked="$(get_song_data '.rating.liked')"

	if [[ ${title} = "null" ]]; then # player not active, don't print anything
		echo ""
	else
		current_time="$(get_song_data '.time.current')"
		total_time="$(get_song_data '.time.total')"

		mseconds_left="$((${total_time} - ${current_time}))"
		seconds_left="$((${mseconds_left} / 1000))"
		minutes="$((${seconds_left}/60))"
		seconds="$((${seconds_left}%60))"
		seconds="$(printf %02d ${seconds})" # format seconds to always have two digits ie. '07'

		song="${title} - ${artits}"
		time_left="${minutes}:${seconds}"

		if [[ ${liked} = 'true' ]]; then
		    metadata=" ${song} ${time_left}"
	    	else
		    metadata="${song} ${time_left}"
		fi

		if [[ ${player_status} = "true" ]]; then
			echo "${playing} ${metadata}"
		else
			echo "${paused} ${metadata}"
		fi
	fi

	return 0
}

get_time()
{
	icon=""
	time=$(date '+%l:%M %p')

	if [[ ${time:0:1} == " " ]]; then
		echo "${icon}${time}"
	else
		echo "${icon} ${time}"
	fi

	return 0
}

get_volume()
{
	speaker=""
	muted=""
	volume=$(amixer get Master | awk -F'[][]' 'END{ print $2 }')
	status=$(amixer get Master | awk 'FNR == 6 { print $6 }')

	if [[ ${status} == "[on]" ]]; then
		echo "${speaker} ${volume}"
	else
		echo "${muted} ${volume}"
	fi

	return 0
}

if get_battery ; then # battery to display / is a laptop
	while true;
	do
		xsetroot -name "$(get_song)   $(get_volume)  $(get_battery)  $(get_date)  $(get_time)"
		#sleep 1;
	done;
else
	while true;
	do
		xsetroot -name "$(get_song)   $(get_volume)  $(get_date)  $(get_time)"
		#sleep 1;
	done;
fi

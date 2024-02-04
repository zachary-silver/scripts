#!/bin/bash

## WM Name
windowManagerName () {
    local window=$(
        xprop -root -notype
    )

    local identifier=$(
        echo "${window}" |
        awk '$1=="_NET_SUPPORTING_WM_CHECK:"{print $5}'
    )

    local attributes=$(
        xprop -id "${identifier}" -notype -f _NET_WM_NAME 8t 2> /dev/null
    )

    local name=$(
        echo "${attributes}" |
        grep "_NET_WM_NAME = " |
        cut --delimiter=' ' --fields=3 |
        cut --delimiter='"' --fields=2
    )

    if [ -z "${name}" ]; then
	    name="dwm"
    fi

    echo "${name}"
}

## Get system info
hostname="$(hostnamectl | head -n 1 | awk '{ print $3 }')"
user="${USER}@${hostname}"
distro='Arch'
kernel="$(uname -sr | sed 's/-.*//')"
uptime="$(uptime -p | sed 's/up //')"
shell="$(basename ${SHELL})"
packages="$(pacman -Qq | wc -l)"

if [ -z "${WM}" ]; then
		envtype='WM'
        WM="$(windowManagerName)"
fi

## Define Colours
if [[ $1 = "--red" ]]; then
    bgaccent="$(tput setab 1)"
    accent="$(tput setaf 1)"
elif [[ $1 = "--green" ]]; then
    bgaccent="$(tput setab 2)"
    accent="$(tput setaf 2)"
elif [[ $1 = "--yellow" ]]; then
    bgaccent="$(tput setab 3)"
    accent="$(tput setaf 3)"
elif [[ $1 = "--blue" ]]; then
    bgaccent="$(tput setab 4)"
    accent="$(tput setaf 4)"
elif [[ $1 = "--magenta" ]]; then
    bgaccent="$(tput setab 5)"
    accent="$(tput setaf 5)"
elif [[ $1 = "--cyan" ]]; then
    bgaccent="$(tput setab 6)"
    accent="$(tput setaf 6)"
else
    bgaccent="$(tput setab 1)"
    accent="$(tput setaf 1)"
fi

bold="$(tput bold)"
white="$(tput setaf 7)"
yellow="$(tput setaf 3)"
red="$(tput setaf 1)"
blue="$(tput setaf 4)"
green="$(tput setaf 2)"
orange="$(tput setaf 8)"
violet="$(tput setaf 5)"
cyan="$(tput setaf 6)"
reset="$(tput sgr0)"
cbg="${reset}${bold}${bgaccent}${white}"
cr="${reset}"
c0="${reset}${bold}"
c1="${reset}${accent}"

## Output

cat <<EOF

${c0}${white}       /\       ${c0}${white}    USER${reset}       ${c0}${violet}${user}${reset}
${c0}${white}      /  \      ${c0}${white}    DISTRO     ${c0}${red}${distro}${reset}
${c0}${white}     /\   \     ${c0}${white}    KERNEL     ${c0}${yellow}${kernel}${reset}
${c0}${white}    /      \    ${c0}${white}    UPTIME     ${c0}${cyan}${uptime}${reset}
${c0}${white}   /   ,,   \   ${c0}${white}    ${envtype}         ${c0}${orange}${WM}${reset}
${c0}${white}  /   |  |  -\  ${c0}${white}    SHELL      ${c0}${green}${shell}${reset}
${c0}${white} /_-''    ''-_\ ${c0}${white}    PACKAGES   ${c0}${blue}${packages}${reset}
EOF

f=3 b=4
for j in f b; do
  for i in {0..7}; do
    printf -v $j$i %b "\e[${!j}${i}m"
  done
done
d=$'\e[1m'
t=$'\e[0m'
v=$'\e[7m'


#pcs() { for i in {0..7}; do echo -en "\e[${1}$((30+$i))m\u2584 \e[0m"; done; }
#printf "\n\t\t    %s\n" "$(pcs)"
printf "\n"

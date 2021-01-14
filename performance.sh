#!/bin/bash

if [ ! $# -eq 2 ] ; then
	echo "Usage: performance.sh [command] [iterations]"
	exit 1
fi

total=0.0

for (( i=0; i<$2; i++ ))
do
    results=$({ time $1 ; } 2>&1)
    user=$(echo $results | awk '{print $4}')
    user=${user::-1}
    m_index=$(echo $user | grep -b -o 'm')
    m_index=${m_index:0:1}
    user_minutes=${user:0:$m_index}
    user_seconds=${user:$m_index}
    user_seconds=${user_seconds:1}
    sys=$(echo $results | awk '{print $6}')
    sys=${sys::-1}
    m_index=$(echo $sys | grep -b -o 'm')
    m_index=${m_index:0:1}
    sys_minutes=${sys:0:$m_index}
    sys_seconds=${sys:$m_index}
    sys_seconds=${sys_seconds:1}
    total=$(awk "BEGIN {print $total+$sys_minutes+$sys_seconds+$user_minutes+$user_seconds; exit}")
done

echo "Average \"$1\" combined system and user run time: $(awk "BEGIN {print $total/$2; exit}")"

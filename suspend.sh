#!/bin/bash

lock()
{
	$HOME/.scripts/lock.sh
}

lock && sleep .5 && systemctl suspend

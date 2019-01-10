#!/bin/bash

lock()
{
	$HOME/.scripts/lock.sh
}

lock && systemctl suspend

#!/bin/bash

lock_script=$HOME/scripts/lock.sh

$lock_script && sleep .5 && systemctl suspend

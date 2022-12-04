#!/bin/sh
# (C) 2019 minim.co
source onboard.sh

op_mode=`/sbin/uci get nearhop.@unum[0].opmode`
if [ "$op_mode" != "" ]; then
	# op_mode has been already set. 
	# Meaning the unit has been already onboarded
	exit 0
fi

SLEEP_TIME=0
MAX_CONN_WAIT_TIME=300
PENALTY_POINTS=0
MAX_PENALTY_POINTS=6 # 30 seconds (6 * 5sec)
MAX_DURATION=86400 # 24 Hours
duration=0
restart_network=1
conn_wait_time=0

# Come out of the following loop only when -
# 1. It reaches a maximum duration of 24 hours
# 2. Device received the config

while true
do
	op_mode=`/sbin/uci get nearhop.@unum[0].opmode`
	if [ "$op_mode" != "" ]; then
		# op_mode has been already set. 
		# Meaning the unit has been already onboarded
		exit 0
	fi
	SLEEP_COUNTER=0
	while [ $SLEEP_COUNTER -lt $SLEEP_TIME ]; do
		sleep 1
		let "led_state = 0$led_state ^ 1"
		set_mesh_led_state $led_state
		SLEEP_COUNTER=`expr $SLEEP_COUNTER + 1`
	done
	let "duration += $SLEEP_TIME"

	# default sleep time for the next iteration (see MAX_PENALTY_POINTS if
	# changing this value)
	SLEEP_TIME=5

	# Reached maximum duration and no need to bypass the time check
	if [ $duration -gt $MAX_DURATION ]; then
		logger "Reached a duration of $duration  seconds. \
				Exit Provisioning loop"
		break
	fi

	wired=`/sbin/is_wired_network.sh`

	if [ 0$wired -eq 0 ]; then
		# Check if the STA is connected to the onboarding SSID
		# If it is not connected assume we are not onboarding
		bssid=`iw sta0 link | grep "Connected to" | awk '{print $3}'`
		if [ "$bssid" == "" ]; then
			PENALTY_POINTS=0
			continue
		fi
	fi

	rootgw=`ip route show | grep default | \
						awk '{print $3}'`
	if [ "$rootgw" == "" ]; then
		PENALTY_POINTS=`expr $PENALTY_POINTS + 1`
		continue
	fi
	root_ping=`ping -q -4 -c 1  $rootgw | \
			grep "1 packets received" | wc | \
			awk '{print $1}'`
	if [ "$root_ping" == "" ]; then
		PENALTY_POINTS=`expr $PENALTY_POINTS + 1`
		continue
	fi

	onboarding=`/sbin/is_onboarding_open.sh $rootgw`
	if [ "$onboarding" != "YES" ]; then
		continue
	fi
	/sbin/onboarded.sh "ap"

done

#!/bin/sh
counter=0
while true
do
	rootgw=`ip route show | grep default | \
						awk '{print $3}'`
	if [ "$rootgw" != "" ]; then
		echo $rootgw
		exit 0
	fi
	counter=`expr $counter + 1`
	if [ $counter -gt 2 ]; then
		logger "Tried maximum times. No trace of route. Exiting"
		exit
	fi
	sleep 5
done

#!/bin/sh
exit 0
opmode=$1
if [ "$opmode" == "" ]; then
	exit 1
fi
if [ "$opmode" != "ap" ] && [ "$opmode" != "gw" ]; then
	exit 1
fi
curopmode=`/sbin/uci get nearhop.@unum[0].opmode`
if [ "$opmode" == "$curopmode" ]; then
	# No change in the opmode
	exit 0
fi

/sbin/create_guest.sh
/sbin/create_mesh.sh
logger "Onboarding device with mode "$opmode
if [ "$opmode" == "ap" ]; then
	while true
	do
		logger "Initial sync of the config from root"
		# If we are here then there is connectivity to the root,
		# Try to get the settings
		error=`/sbin/sync_config.sh 1`
		if [ "$error" != "" ]; then
			logger "Onboarding error "$error
			continue
		else
			logger "Sycned config successfully from root"
			break
		fi
		sleep 5
	done
fi

if [ "$opmode" == "gw" ]; then
	/sbin/create_vpn_forward_firewall.sh
elif [ "$opmode" == "ap" ]; then
	settings=`/usr/sbin/nearhopd-cli -ipaddress 127.0.0.1 -command wireless_set -data /tmp/settings.txt`
	error=`echo $settings | grep ^Error`
	if [ "$error" != "" ]; then
		logger "Error while applying the settings "$error
		exit 0
	fi
	/sbin/set_repeater.sh
fi
/sbin/uci set nearhop.@unum[0].opmode=$opmode
/sbin/uci commit nearhop
/sbin/create_on_board_sta.sh # This will remove if there is any onbording interface
/etc/init.d/dnsmasq restart
/etc/init.d/network restart
sleep 15
/etc/init.d/nearhopd restart
if [ "$opmode" == "ap" ]; then
	/etc/init.d/unum restart
fi
ps | grep sync_config.sh | grep -v grep | awk '{print $1}' | xargs kill
/sbin/sync_config.sh &
/sbin/register_repeater.sh &

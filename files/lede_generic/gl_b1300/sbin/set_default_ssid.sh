#!/bin/sh

source m_functions.sh

[ -f /tmp/wireless_exists.txt ] ||
#Special cases
bname=`cat /tmp/sysinfo/board_name`
case $bname in
glinet,gl-b1300)
	cc=$(get_b1300_country_code)
	mac_suffix=$(get_ipq40xx_mac_suffix)

	# For GL-Inet's B1300, ssid is GL-B1300- followed by
	# last 3 niblles of MAC Address
	ssid="GL-B1300-"$mac_suffix
	# key last 8 characters of serial Number
	key="goodlife"

	#Set the default SSID and Key for both the radios
	for devidx in 0 1
	do
		htmode=""
		channel=""
		band=`/sbin/uci get wireless.radio${devidx}.band`
		if [ "$band" == "5g" ]; then
			htmode="VHT80"
			channel="36"
		else
			htmode="HT20"
			channel="1"
		fi
		uci -q batch <<-EOF
			set wireless.default_radio${devidx}.ssid=$ssid
			set wireless.default_radio${devidx}.encryption="psk2"
			set wireless.default_radio${devidx}.key=$key
			set wireless.default_radio${devidx}.disabled=0
			set wireless.radio${devidx}.disabled=0
			set wireless.radio${devidx}.country="US"
			set wireless.radio${devidx}.htmode=$htmode
			set wireless.radio${devidx}.channel=$channel

EOF
	done
;;
*)
esac
uci -q commit wireless
rm /tmp/wireless_exists.txt

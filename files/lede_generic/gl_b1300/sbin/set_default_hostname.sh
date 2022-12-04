#!/bin/sh
source m_functions.sh
mac_suffix=$(get_ipq40xx_mac_suffix)
hostname="OpenWRT"$mac_suffix
/sbin/uci set system.@system[0].hostname=$hostname
/sbin/uci commit system
/etc/init.d/system restart

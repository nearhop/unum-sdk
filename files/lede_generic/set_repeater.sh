#!/bin/sh
/sbin/uci set dhcp.lan.ignore=1
/sbin/uci commit dhcp

/sbin/uci set network.lan.proto="dhcp"
/sbin/uci commit network

/sbin/set_repeater_platform.sh

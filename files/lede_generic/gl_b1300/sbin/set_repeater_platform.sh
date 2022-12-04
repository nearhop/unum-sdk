#!/bin/sh
/sbin/uci set network.@device[0].ports="eth0 eth1"
/sbin/uci delete network.wan
/sbin/uci commit network

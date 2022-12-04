#!/bin/sh
# (C) 2022 nearhop.com
startap=$1
source onboard.sh

if [ 0$startap -eq 1 ]; then
	create_onboard_ap 1 "NearhopOnboard" "Ne@R409one"
else
	create_onboard_ap 0
fi
/sbin/wifi up &

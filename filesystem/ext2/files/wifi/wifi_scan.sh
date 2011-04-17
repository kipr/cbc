#!/bin/sh

WIFI=`cat /proc/net/wireless | sed -n 3p`
WIFI=${WIFI%%:*}

if [ "${WIFI}" != "rausb0" ]; then
	[ ! -e /mnt/kiss/wifi/ssids ] || { rm /mnt/kiss/wifi/ssids; }
	# No wireless card is connected
	exit 2 
fi

# startup the wireless if not already up
WIFIUP=`ifconfig | grep rausb0`
if [ -z "$WIFIUP" ]; then
	ifconfig rausb0 up
fi
	
# scan for all ssid's on the wireless 
# and replace the "Cell" string with \n
# and remove the first line then save to file ssids
echo `iwlist rausb0 scan` | sed 's/Cell /\n/g' | awk 'FNR>1' > /mnt/kiss/wifi/ssids

if [ -s /mnt/kiss/wifi/ssids ]; then
	# ssid list has been created
	exit 0
fi
# wifi is connected but no ssids are available
exit 1

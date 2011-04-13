#!/bin/sh

WIFI=`cat /proc/net/wireless | sed -n 3p`
WIFI=${WIFI%%:*}

if [ "${WIFI}" != "rausb0" ]; then
	[ ! -e ssids ] || { rm ssids; }
	exit 1
fi

# startup the wireless if not already up
WIFIUP=`ifconfig | grep rausb0`
if [ -z "$WIFIUP" ]; then
	ifconfig rausb0 up
fi
	
# scan for all ssid's on the wireless 
# and replace the "Cell" string with \n
# and remove the first line then save to file ssids
echo `iwlist rausb0 scan` | sed 's/Cell /\n/g' | awk 'FNR>1' > ssids


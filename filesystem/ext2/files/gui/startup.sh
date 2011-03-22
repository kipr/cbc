#!/bin/sh
cd /mnt/kiss

source /mnt/kiss/config/ts_vars.env

if [ ! -e ${TSLIB_CALIBFILE} ]; then
	/mnt/kiss/qt/bin/ts_calibrate
	sync
	sync
fi

./cbcui -qws


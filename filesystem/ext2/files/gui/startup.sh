#!/bin/sh
cd /mnt/kiss

source ./config/ts_vars.env

if [ ! -e ${TSLIB_CALIBFILE} ]; then
	./qt/bin/ts_calibrate
	sync
	sync
fi

./gui/cbcui -qws &


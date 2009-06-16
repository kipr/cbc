#!/bin/sh

KISS=/mnt/kiss
QT=${KISS}/qt

export TSLIB_TSDEVICE=/dev/input/event0
export TSLIB_FBDEVICE=/dev/fb0
export TSLIB_PLUGINDIR=${QT}/lib/ts
export TSLIB_CONSOLEDEVICE=/dev/ttyS0
export TSLIB_CONFFILE=${QT}/etc/ts.conf
export TSLIB_CALIBFILE=/psp/rfs1/pointercal
export LD_LIBRARY_PATH=/lib:${QT}/lib
export POINTERCAL_FILE=${TSLIB_CALIBFILE}
export QWS_MOUSE_PROTO=Tslib:/dev/input/event0

if [ ! -e ${TSLIB_CALIBFILE} ]; then
  ${QT}/bin/ts_calibrate
  sync
  sync
fi

$@

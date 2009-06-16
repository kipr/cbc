#!/bin/sh
cd /mnt/kiss
echo "" | /mnt/usb/fb_print --echo
echo "Starting cbcui..." | /mnt/usb/fb_print --echo
./run_qt_app.sh ./cbcui -qws


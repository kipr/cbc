#!/bin/sh

umount /mnt/user
save_time
sleep 3
sync
sync
cpi -s

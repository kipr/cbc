#!/bin/sh




#DEVICE=$1
#YESNO="n"
#


# this is pretty unsafe
DEVICES=`ls /dev/sd? | grep -v sda`


echo "You are about to wipe all of the data on the following drives:"
echo ${DEVICES}
echo
echo "Are you sure? (y/N)"
read YESNO

if [ -z "${YESNO}" -o "${YESNO}" != "y"  ]; then
    echo "canceling"
    exit 1;
fi


for DEVICE in $DEVICES
do
    echo "Partitioning drive... ${DEVICE}"
    if [ ! -e "${DEVICE}" ]; then
	echo "specified device ${DEVICE} doesn't exist, canceling"
	exit 1;
    fi
    
    umount ${DEVICE}
(
    dd if=/dev/zero of=${DEVICE} bs=512 count=1
    fdisk ${DEVICE} < fdisk.script
    sudo mkfs.vfat ${DEVICE}1
    sudo mkfs.ext2 ${DEVICE}2
)&

done;

wait


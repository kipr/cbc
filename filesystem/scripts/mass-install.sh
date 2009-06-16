#!/bin/bash


SRC="/home/kipr/Desktop/filesystem"


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

    if [ ! -e "${DEVICE}"  ]; then
	echo "specified device ${DEVICE} doesn't exist"
	exit 1;
    fi

    if [ ! -e "${DEVICE}1" -o ! -e "${DEVICE}2" ]; then
	echo "specified device ${DEVICE}  isn't partitioned correctly"
	exit 1;
    fi

	umount ${DEVICE}1
	umount ${DEVICE}2


done;

count=1
for DEVICE in ${DEVICES}
do
    
	echo "Installing to drive... ${DEVICE}"

	UPATH="/mnt/d${count}-u"
	KPATH="/mnt/d${count}-k"
	
	echo "Mounting ${DEVICE}1 at ${UPATH}"
	echo "Mounting ${DEVICE}2 at ${KPATH}"

	if [ -x ${UPATH} ]
	then
	    mount ${DEVICE}1 ${UPATH}
	    rm -rf ${UPATH}/*
	(   cp -r  ${SRC}/kiss-vfat/* ${UPATH} )&
	fi
	
	if [ -x ${KPATH} ]
	then
	    mount ${DEVICE}2 $KPATH
	    rm -rf ${KPATH}/*
	(   cp -a  ${SRC}/kiss-ext2/* ${KPATH} )&
	fi
	


    count=`expr $count + 1`

done

wait

echo "Finished with drives:"
echo {$DEVICES}
echo
echo "Remember to unmount them"
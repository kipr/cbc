#!/bin/sh

if [ "`whoami`" != "root" ]; then
  echo "You must be root to run this script"
  exit 1
fi

DEVICE=$1
YESNO="n"

if [ "$DEVICE" = "" ]; then
  echo "Usage: sudo partition.sh /dev/usbkeydevice"
  exit 1
fi

echo "You are about to wipe all of the data on drive ${DEVICE}"
echo "Are you sure? (y/N)"
read YESNO

if [ -z "${YESNO}" -o "${YESNO}" != "y"  ]; then
  exit 1;
fi

if [ ! -e "${DEVICE}" ]; then
  exit 1;
fi

echo "Partitioning drive..."
dd if=/dev/zero of=${DEVICE} bs=512 count=1
fdisk ${DEVICE} < fdisk.script

mkfs.vfat ${DEVICE}1
mkfs.ext2 ${DEVICE}2

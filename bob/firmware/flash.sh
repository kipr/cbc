#!/bin/sh
cp -f bin/firmware-flash.bin main.bin
openocd -f openocd-flash.cfg

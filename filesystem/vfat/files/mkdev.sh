#!/bin/sh

rm -rf /dev/cbc
mkdir -p /dev/cbc

# Digital ports
mknod /dev/cbc/digital8  c 220 0
mknod /dev/cbc/digital9  c 220 1
mknod /dev/cbc/digital10 c 220 2
mknod /dev/cbc/digital11 c 220 3
mknod /dev/cbc/digital12 c 220 4
mknod /dev/cbc/digital13 c 220 5
mknod /dev/cbc/digital14 c 220 6
mknod /dev/cbc/digital15 c 220 7
mknod /dev/cbc/button    c 220 8
mknod /dev/cbc/digital   c 220 9

# Analog ports
mknod /dev/cbc/analog0 c 221 0
mknod /dev/cbc/analog1 c 221 1
mknod /dev/cbc/analog2 c 221 2
mknod /dev/cbc/analog3 c 221 3
mknod /dev/cbc/analog4 c 221 4
mknod /dev/cbc/analog5 c 221 5
mknod /dev/cbc/analog6 c 221 6
mknod /dev/cbc/analog7 c 221 7
mknod /dev/cbc/battery c 221 8
mknod /dev/cbc/analog  c 221 9

# Generic sensor poll
mknod /dev/cbc/sensors c 222 0

mknod /dev/cbc/pwm0 c 223 0
mknod /dev/cbc/pwm1 c 223 1
mknod /dev/cbc/pwm2 c 223 2
mknod /dev/cbc/pwm3 c 223 3
mknod /dev/cbc/pwm  c 223 4

mknod /dev/cbc/pid0 c 224 0
mknod /dev/cbc/pid1 c 224 1
mknod /dev/cbc/pid2 c 224 2
mknod /dev/cbc/pid3 c 224 3
mknod /dev/cbc/pid  c 224 4

mknod /dev/cbc/servo0 c 225 0
mknod /dev/cbc/servo1 c 225 1
mknod /dev/cbc/servo2 c 225 2
mknod /dev/cbc/servo3 c 225 3
mknod /dev/cbc/servo  c 225 4

mknod /dev/cbc/accelX c 226 0
mknod /dev/cbc/accelY c 226 1
mknod /dev/cbc/accelZ c 226 2
mknod /dev/cbc/accel  c 226 3

mknod /dev/cbc/status c 228 0

mknod /dev/cbc/uart0 c 227 0
mknod /dev/cbc/uart1 c 227 1

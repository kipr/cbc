#Onurs address pass:RouterAdm1n
#CBC_IP=192.168.1.75
#KIPR address
CBC_IP=192.168.123.173
make &&
ssh root@$CBC_IP killall cbcui 
scp cbcui root@$CBC_IP:/tmp/cbcui &&
ssh root@$CBC_IP /mnt/kiss/run_qt_app.sh /tmp/cbcui -qws

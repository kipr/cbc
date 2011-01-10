#Onurs address pass:RouterAdm1n
#CBC_IP=192.168.1.76
#KIPR address
CBC_IP=192.168.123.182
make &&
ssh root@$CBC_IP killall cbcui 
scp cbcui root@$CBC_IP:/tmp/cbcui &&
echo -e "\a" &&
ssh root@$CBC_IP /mnt/kiss/run_qt_app.sh /tmp/cbcui -qws

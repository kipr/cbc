#Onurs address pass:RouterAdm1n
#CBC_IP=192.168.1.76
#KIPR address
CBC_IP=192.168.123.56
make &&
ssh root@$CBC_IP killall cbcui 
scp cbcui root@$CBC_IP:/mnt/kiss/gui/cbcui &&
ssh root@$CBC_IP /mnt/kiss/gui/startup.sh

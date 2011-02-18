#Onurs address pass:RouterAdm1n
#CBC_IP=192.168.1.78
#KIPR address
CBC_IP=192.168.123.56
make &&
ssh root@$CBC_IP killall cbcui 
scp cbcui root@$CBC_IP:/tmp/cbcui
# &&
#ssh root@$CBC_IP source /mnt/kiss/ts_vars.env /tmp/cbcui -qws

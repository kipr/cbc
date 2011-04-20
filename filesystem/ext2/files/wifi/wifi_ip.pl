#!/usr/bin/perl

my $iface = `cat /proc/net/wireless | sed -n 3p`;
$iface = substr $iface, 0, index($iface,':');

# check for an ip address
if( $iface eq "" ){ exit 1; }
my $IP = `ifconfig $iface | sed -n 2p`;
my $colon = index($IP,':') + 1;
$IP = substr $IP, $colon, 15;
if( $IP eq "" ){ exit 1; }
print $IP;
exit 0;

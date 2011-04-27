#!/usr/bin/perl

my $iface = `cat /proc/net/wireless | sed -n 3p`;
$iface = substr $iface, 0, index($iface,':');

# check for an ip address
if( $iface eq "" ){ exit 1; }
my $IP = `ifconfig $iface | sed -n 2p`;
if( $IP =~ m/inet addr:/s ){
	my $colon = index($IP,':') + 1;
	my $space = index($IP,' ',$colon);
	$IP = substr $IP, $colon, ($space - $colon);
	if( $IP eq "" ){ exit 1; }
	print $IP;
	exit 0;
}
print "No IP from DHCP";
exit 1;

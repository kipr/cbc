#!/usr/bin/perl

my $cbcNetConfig = "/psp/cbc_net_config";
my $iface = `cat /proc/net/wireless | sed -n 3p` || "";
my %netConfig;

$iface = substr $iface, 0, index($iface,':');

# check for network config file
if( ! -e $cbcNetConfig )
{ print "Network config file not found\n"; exit 3; }

# check for a connected device
if( $iface eq "" )
{ print "Network device not connected\n"; exit 2; }

# read in the network configuration file
%netConfig = split(/[=\n]/, `cat $cbcNetConfig`);

#foreach my $var (keys %netConfig)
#{ print "$var: $netConfig{$var}"; }

# bring down the wifi
system( "ifconfig $iface down" );

# kill the ssh daemon
system( "killall sshd 2>&1" );

if( ! defined( $netConfig{'type'} ) )
{ $netConfig{'type'} = "wlan"; }

sleep( 1 );

# bring up the network device
system( "ifconfig $iface up" );

# be sure to scan for new networks, apparently the wireless system
# needs to do this before connecting to a new network
system( "iwlist $iface scan 2>&1" );

if( $netConfig{'allocation'} eq "dhcp" )
{
	# kill the dhcp process
	system( "killall udhcpc 2>&1" );
	
	if( $netConfig{'type'} ne "wlan" )
	{
		system( "udhcpc -t 5 -n -p /var/run/udhcpc.$iface.pid -i $iface" );
		system( "/sbin/sshd" );
		exit 0;
	}
}
else
{
	#print "setup Static network\n";
	# setup static allocation
	if( ! defined( $netConfig{'ip'} ) )
	{ print "Static IP not set\n"; exit 3; }
	
	if( ! defined( $netConfig{'netmask'} ) )
	{ print "Static Netmask not set\n"; exit 3; }
	
	if( ! defined( $netConfig{'gateway'} ) )
	{ print "Static Gateway not set\n"; exit 3; }

	system( "ifconfig $iface $netConfig{'ip'} netmask $netConfig{'netmask'}" );
	
	my $successDel = 0;
	while( $successDel == 0 )
	{
		system( "route del default gw 0.0.0.0 dev $iface 2>/dev/null" );
		$successfulDel = $?;
	}
	system( "route add default gw $netConfig{'gateway'} dev $iface" );
	open( F, ">/tmp/resolv.conf" );
	print F "nameserver $netConfig{'nameserver1'}\n";
	print F "nameserver $netConfig{'nameserver2'}\n";
	close( F );
	
	if( $netConfig{'type'} ne "wlan" )
	{
		system( "/sbin/sshd" );
		exit 0;
	}
}

# setup Adhoc or Managed Infra networks
if( $netConfig{'netType'} eq "Adhoc" )
{ system( "iwconfig $iface mode Ad-Hoc" ); }
else
{ system( "iwconfig $iface mode Managed" ); }

# set the Bit rate transmission speed
if( $netConfig{'txrate'} eq "" )
{ $netConfig{'txrate'} = "9Mb/s"; }
system( "iwconfig $iface rate $netConfig{'txrate'}" );

system( "iwpriv $iface set AuthMode=$netConfig{'auth'}" );
system( "iwpriv $iface set SSID=$netConfig{'ssid'}" );
sleep( 1 );

#print "check auth is WPA or WEP\n";
if( ( $netConfig{'auth'} eq "WPAPSK" ) || ( $netConfig{'auth'} eq "WPA2PSK" ) )
{
	#print "auth is WPA\n";
	system( "iwpriv $iface set WPAPSK=$netConfig{'key'}" );
	system( "iwpriv $iface set SSID=$netConfig{'ssid'}" );
	sleep( 1 );
	
	#print "determine encryption type\n";
	my $count = 1;

	# if not set, default is AES
	if( ! defined( $netConfig{'encryption'} ) )
	{ $netConfig{'encryption'} = "AES"; }

ENCRYP:
	system( "iwpriv $iface set EncrypType=$netConfig{'encryption'}" );
	system( "iwpriv $iface set SSID=$netConfig{'ssid'}" );
	sleep( 1 );
	
	if( $netConfig{'ssid'} ne `/mnt/kiss/wifi/wifi_connected.pl` )
	{
		#print "encryption failed\n";
		if( $netConfig{'encryption'} eq "AES" )
		{ $netConfig{'encryption'} = "TKIP"; }
		else
		{ $netConfig{'encryption'} = "AES"; }
		
		if( $count == 1 ){
			++$count;
			goto ENCRYP;
		}else{
			print "Could not determine Encryption\n";
			exit 3;
		}
	}
}
elsif( $netConfig{'encryption'} eq "WEP" )
{
	#print "auth is WEP\n";
	system( "iwpriv $iface set EncrypType=$netConfig{'encryption'}" );
	system( "iwpriv $iface set SSID=$netConfig{'ssid'}" );
	if( $netConfig{'encoding'} eq "ascii" )
	{ determineWepKey( ); }
	system( "iwpriv $iface set Key1=$netConfig{'key'}" );
	system( "iwpriv $iface set SSID=$netConfig{'ssid'}" );
	sleep( 1 );
}
else
{
	system( "iwpriv $iface set EncrypType=$netConfig{'encryption'}" );
	system( "iwpriv $iface set SSID=$netConfig{'ssid'}" );
	sleep( 1 );
}

if( $netConfig{'ssid'} eq `/mnt/kiss/wifi/wifi_connected.pl` )
{
	if( $netConfig{'allocation'} eq "dhcp" )
	{ 
		system( "udhcpc -t 5 -n -p /var/run/udhcpc.$iface.pid -i $iface" ); 
		my $IP = `/mnt/kiss/wifi/wifi_ip.pl $iface`;
		if( $IP eq "" ) { print "IP not allocated"; exit 3; }
		$netConfig{'ip'} = $IP;
	}
	# start the ssh daemon
	system( "/sbin/sshd" );
	
	writeConfigFile( );
	print "Connected!\n";
	exit 0;
}

print "Not connected\n";
exit 3;

sub writeConfigFile
{
	open( F, ">$cbcNetConfig" );

	foreach my $key (keys %netConfig )
	{
		print F "$key=$netConfig{$key}\n";
	}
	close( F );
}

sub determineWepKey
{
	my $asciiPasskey = $netConfig{'key'};
	my @wepKeys = `CHUMBY_BINS=/usr/bin perl /usr/chumby/scripts/weppasswd.pl $asciiPasskey`;
	chomp( @wepKeys );

	# if the password entered is exactly 10 or 26 HEX chars, add them to the front of the list
	if( $netConfig{'key'}=~ m/[0-9A-F]{10}|[0-9A-F]{26}/i )
	{ unshift( @wepKeys, $netConfig{'key'} ); }

	my $i = 0;
	my $total = @wepKeys;
	foreach my $key ( @wepKeys )
	{
		++$i;
		print "<status key=\"$key\" index=\"$i\" total=\"$total\"/>\n";
	
		system( "iwpriv $iface set AuthMode=$netConfig{'auth'}" );
		system( "iwpriv $iface set SSID=$netConfig{'ssid'}" );
		system( "iwpriv $iface set Key1=" . $key );
		system( "iwpriv $iface set SSID=$netConfig{'ssid'}" );
		
		if( $netConfig{'ssid'} eq `/mnt/kiss/wifi/wifi_connected.pl` )
		{
			if( $netConfig{'allocation'} eq "dhcp" )
			{ system( "udhcpc -t 5 -n -p /var/run/udhcpc.$iface.pid -i $iface" ); }
			
			my $netStatus = `/usr/chumby/scripts/network_status.sh`;
			if( ! ( $netStatus =~ /error/ ) )
			{
				$netConfig{'key'} = $key;
				$netConfig{'encoding'} = "hex";
				return;
			}
		}
	} 
}


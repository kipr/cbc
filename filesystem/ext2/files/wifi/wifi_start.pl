#!/usr/bin/perl

my $cbcNetConfig = "/psp/cbc_net_config";
my $iface = `cat /proc/net/wireless | sed -n 3p` || "";
my %netConfig;

$iface = substr $iface, 0, index($iface,':');

# check for network config file
if( ! -e $cbcNetConfig )
{ print "Network config file not found\n"; exit 1; }

# check for a connected device
if( $iface eq "" )
{ print "Network device not connected\n"; exit 1; }

# read in the network configuration file
%netConfig = split(/[=\n]/, `cat $cbcNetConfig`);

#foreach my $var (keys %netConfig)
#{ print "$var: $netConfig{$var}"; }

if( $netConfig{'ssid'} eq `/mnt/kiss/wifi/wifi_connected.pl` )
{ print "Already connected to:$netConfig{'ssid'}"; exit 0; }

if( ! defined( $netConfig{'type'} ) )
{ $netConfig{'type'} = "wlan"; }

# bring up the network device
system( "ifconfig $iface up" );

# kill the ssh daemon
system( "killall ssh 2>&1" );

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
	{ print "Static IP not set\n"; exit 1; }
	
	if( ! defined( $netConfig{'netmask'} ) )
	{ print "Static Netmask not set\n"; exit 1; }
	
	if( ! defined( $netConfig{'gateway'} ) )
	{ print "Static Gateway not set\n"; exit 1; }

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

if( ! defined( $netConfig{'txrate'} ) )
{ $netConfig{'txrate'} = 6; }

system( "iwpriv $iface set TxRate=$netConfig{'txrate'}" );
system( "iwpriv $iface set AuthMode=$netConfig{'auth'}" );
system( "iwpriv $iface set SSID=$netConfig{'ssid'}" );

#print "check auth is WPA or WEP\n";
if( ( $netConfig{'auth'} eq "WPAPSK" ) || ( $netConfig{'auth'} eq "WPA2PSK" ) )
{
	#print "auth is WPA\n";
	system( "iwpriv $iface set WPAPSK=$netConfig{'key'}" );
	system( "iwpriv $iface set SSID=$netConfig{'ssid'}" );

	#print "determine encryption type\n";
	my $count = 1;

	# if not set, default is AES
	if( ! defined( $netConfig{'encryption'} ) )
	{ $netConfig{'encryption'} = "AES"; }

ENCRYP:
	system( "iwpriv $iface set EncrypType=$netConfig{'encryption'}" );
	system( "iwpriv $iface set SSID=$netConfig{'ssid'}" );

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
			exit 1;
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
}

if( $netConfig{'ssid'} eq `/mnt/kiss/wifi/wifi_connected.pl` )
{
	if( $netConfig{'allocation'} eq "dhcp" )
	{ 
		system( "udhcpc -t 5 -n -p /var/run/udhcpc.$iface.pid -i $iface" ); 
		my $IP = `ifconfig $iface | sed -n 2p`;
		my $colon = index($IP,':') + 1;	
		$IP = substr $IP, $colon, 15;
		if( $IP eq "" ) { print "IP not allocated"; exit 1; }
		$netConfig{'ip'} = $IP;
	}
	# start the ssh daemon
	system( "/sbin/sshd" );
	
	writeConfigFile( );
	print "Connected!\n";
	exit 0;
}

print "Not connected\n";
exit 1;

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


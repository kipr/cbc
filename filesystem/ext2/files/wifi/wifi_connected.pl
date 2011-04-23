#!/usr/bin/perl

my $iface = `cat /proc/net/wireless | sed -n 3p`;
$iface = substr $iface, 0, index($iface,':');
if( $iface eq "" ){ exit 1; }

my $auth = $ARGV[0] || "";

my $timeout 	= 1;
my $i		= 0;
my $AccessPt	= 0;
my $SSID	= 0;

while ( $timeout > $i )
{
	my $iwconfig = `iwconfig $iface 2>&1`;
	if( $iwconfig =~ m/No such device/s )
	{ exit 1; }
	
	# remove all new lines
	$iwconfig =~ s/\n//g;
	# remove all whitespaces greater than 3 and replace with 2 spaces
	$iwconfig =~ s/   */  /g;
	# replace all = with :
	$iwconfig =~ s/=/:/g;
	# split the iwconfig string based on 2 spaces into an array
	my @array = split(/  /,$iwconfig);
	
	$AccessPt = getItemValue( "Access Point: ",@array );
	$SSID = getItemValue( "ESSID:",@array);
	$SSID =~ s/\"//g;
	if( ( $AccessPt ne "" ) && ( $SSID ne "" ) )
	{
		my $EncrypKey = getItemValue( "Encryption Key:",@array );
		if( ( $auth eq "OPEN" ) || ( $EncrypKey ne "off" ) )
		{ print $SSID; exit 0; }
	}
	++$i;
#	sleep( 1 );
}

exit 1;

sub getItemValue
{
	my ( $item, @ary ) = @_;

	my $value = 0;
	foreach my $var (@ary){
		if ( $var =~ m/$item/s )
		{
			$value = substr $var, length($item);
			last;	
		}
	}
	return $value;
}

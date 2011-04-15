#!/usr/bin/perl

$iface = $ARGV[0] || "rausb0";
$auth = $ARGV[1] || "";

my $timeout 	= 4;
my $i		= 0;
my $AccessPt	= 0;
my $SSID	= 0;

while ( $timeout > $i )
{
	my $iwconfig = `iwconfig $iface 2>&1`;
	if( $iwconfig =~ m/No such device/s )
	{
		last;
	}
	
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
	if( ( $AccessPt ne 0 ) && ( $SSID ne 0 ) )
	{
		my $EncrypKey = getItemValue( "Encryption Key:",@array );
		if( $auth eq "OPEN" )
		{
			last;
		}
		elsif ( $EncrypKey ne "off" )
		{
			last;	
		}
	}
	++$i;
	sleep( 1 );
}

if( $SSID eq 0 )
{ 
	exit 1;
}
	
print $SSID,"\n";
exit 0;

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

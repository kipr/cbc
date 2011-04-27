#!/usr/bin/perl

my $ssidFile = "/mnt/kiss/wifi/ssids";

# get the net interface
my $iface = `cat /proc/net/wireless | sed -n 3p`;
$iface = substr $iface, 0, index($iface,':');
if( $iface eq "" ){ unlink $ssidFile; exit 2; }

# startup the wireless if not already up
`ifconfig $iface up`;

# scan for local nets first
my $scan = `iwlist $iface scan`;
$scan =~ s/\n//g;
$scan =~ s/\t/  /g;
$scan =~ s/   */  /g;
$scan =~ s/Cell/\n/g;

# survey the local net connections
my @survey = split( '\n', `iwpriv $iface get_site_survey | awk 'FNR>2' | sed 's/\t/  /g' | sed 's/   */  /g'` );

# check for available connections
if( @survey == 0 ){ unlink $ssidFile; exit 1; }

foreach my $val (@survey)
{
	my @data = split /  /, $val;
	push @data, getQuality( $data[2], $scan );
	push @data, getTxRates( $data[2], $scan );
	$val = join("  " ,@data);
}

# write the list of ssids to a file
open( F, ">$ssidFile" );
foreach my $ssid (@survey)
{
	if( $ssid eq ""){ next; }
	print F $ssid,"\n";
}
close( F );

exit 0;


sub getTxRates
{
	my ( $ssid, $scanData ) = @_;
	
	my $ssidPos = index($scanData, $ssid);
	my $rateStartPos = index($scanData,"Rates",$ssidPos) + 6;
	my $qualityPos = index($scanData,"Quality",$rateStartPos);
	my $rates = substr $scanData, $rateStartPos, ($qualityPos - $rateStartPos);
	$rates =~ s/  /; /g;
	$rates =~ s/ //g;
	return $rates;
}

sub getQuality
{
	my ( $ssid, $scanData ) = @_;
	
	my $ssidPos = index($scanData, $ssid);
	my $qualityStartPos = index($scanData, "Quality", $ssidPos) + 8;
	my $slashPos = index($scanData, '/', $qualityStartPos);
	my $quality = substr $scanData, $qualityStartPos, ($slashPos - $qualityStartPos);
	return $quality;
}

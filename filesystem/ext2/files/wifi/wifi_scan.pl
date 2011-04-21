#!/usr/bin/perl

my $ssidFile = "/mnt/kiss/wifi/ssids";

# get the net interface
my $iface = `cat /proc/net/wireless | sed -n 3p`;
$iface = substr $iface, 0, index($iface,':');
if( $iface eq "" ){ unlink $ssidFile; exit 2; }

# startup the wireless if not already up
`ifconfig $iface up`;

# scan for local nets first
`iwlist $iface scan`;

# survey the local net connections
my $survey = `iwpriv $iface get_site_survey | awk 'FNR>2' | sed 's/\t/  /g' | sed 's/   */  /g'`;

# check for available connections
if( $survey eq ""){ unlink $ssidFile; exit 1; }

# write the list of ssids to a file
chomp( $survey );
open( F, ">$ssidFile" );
print F $survey;
close( F );

exit 0;


#!/usr/bin/perl


my $netstats = `cat /proc/net/wireless | sed -n 3p` || "";

if( $netstats eq "" )
{ exit 1; }

my @array = ($netstats =~ /(\w+)/g);
my $sigQuality = @array[2];
my $sigStrength = @array[3];
my $sigNoise = @array[4];

if( @ARGV[0] eq "" )
{
	print "$sigQuality\n$sigStrength\n$sigNoise\n";
}
elsif( @ARGV[0] =~ m/quality/i )
{
	print "$sigQuality\n";
}
elsif( @ARGV[0] =~ m/strength/i )
{
	print "$sigStrength\n";
}
elsif( @ARGV[0] =~ m/noise/i )
{
	print "$sigNoise\n";
}
else
{
	exit 1;
}

exit 0;

#!/usr/bin/perl -w

use strict;
#my $offset = 8192;
my $offset = (-s "cbc_interface.body") + 2048;
my $ptr_offset = $offset;
my $hookfile = shift(@ARGV);
open(HOOK, '>', $hookfile);

print HOOK "#!/bin/sh\n";
print HOOK "### THIS IS A CHUMBY BOTBALL CONTROLLER INTERFACE UPDATE ###\n";
my $version=`cat FIRMWARE_VERSION`;
chomp $version;
print HOOK "VERSION=$version\n";
print HOOK "SELF=\$0\n";
print HOOK 'echo "#!/usr/bin/perl -w" > /tmp/extract.pl',"\n";
print HOOK 'echo "open IN, \\"<\\$ARGV[0]\\" or die(\\"open error\\");" >> /tmp/extract.pl', "\n";
print HOOK 'echo "seek(IN,\\$ARGV[1],0);" >> /tmp/extract.pl', "\n";
print HOOK 'echo "my \\$count=\\$ARGV[2];" >> /tmp/extract.pl', "\n";
print HOOK 'echo "while (\\$count) {" >> /tmp/extract.pl', "\n";
print HOOK 'echo "\\$count -= read(IN, \\$_, \\$count < 1024 ? \\$count : 1024);" >> /tmp/extract.pl', "\n";
print HOOK 'echo "print; }" >> /tmp/extract.pl', "\n";
print HOOK "chmod +x /tmp/extract.pl\n";

sub write_extract_command
{
    my ($file)=@_;
    (-e $file) or die "can't find $file";
    my $size = (-s $file);
    my $name=$file;
    $name =~ s/\./_/g;
    print HOOK "EXTRACT_$name='/tmp/extract.pl $hookfile $ptr_offset $size'\n";
    $ptr_offset += $size;
}

foreach my $file (@ARGV) {
    write_extract_command($file);
}

print HOOK `cat cbc_interface.body`, "\n";
truncate(HOOK, $offset);
close HOOK;
if ((-s "$hookfile") != $offset) {
    die "$hookfile length != $offset"
    }

foreach my $file (@ARGV) {
    # append each file in the arg list to the end of the cbc interface
    system "cat $file >> $hookfile";
}

if ((-s "$hookfile") != $ptr_offset) {
    die "$hookfile length != $ptr_offset";
}



#!/usr/bin/perl -w

use strict;
my $offset = 8192;

open HOOK, ">userhook0";

print HOOK "#!/bin/sh\n";
print HOOK "### THIS IS A CHUMBY BOTBALL CONTROLLER FIRMWARE UPDATE ###\n";
my $version=`cat FIRMWARE_VERSION`;
chomp $version;
print HOOK "VERSION=$version\n";
print HOOK 'echo "#!/usr/bin/perl -w" > /tmp/extract.pl', "\n";
print HOOK 'echo "open IN, \\"<\\$ARGV[0]\\"; seek(IN,\\$ARGV[1],0); \\$count=\\$ARGV[2]; while (\\$count) { \\$count -= read(IN, \\$_, \\$count < 1024 ? \\$count : 1024); print; }" >> /tmp/extract.pl', "\n";
print HOOK "chmod +x /tmp/extract.pl\n";

sub write_extract_command
{
    my ($file)=@_;
    (-e $file) or die "can't find $file";
    my $size = (-s $file);
    my $name=$file;
    $name =~ s/\./_/g;
#    print HOOK "EXTRACT_$name='dd if=userhook0 bs=1 skip=$offset count=$size'\n";
#    print HOOK "EXTRACT_$name='perl -e \\'open IN, \\\"<userhook0\\\"; seek(IN,$offset,0); \$count=$size; while (\$count) { \$count -= read(IN, \$_, \$count < 1024 ? \$count : 1024); print;}\\''\n";
    print HOOK "EXTRACT_$name='/tmp/extract.pl userhook0 $offset $size'\n";
    $offset += $size;
}

foreach my $file (@ARGV) {
    write_extract_command($file);
}

print HOOK `cat userhook0.body`, "\n";
truncate(HOOK, 8192);
close HOOK;
if ((-s "userhook0") != 8192) {
    die "userhook0 length != 8192"
    }

foreach my $file (@ARGV) {
    system "cat $file >> userhook0";
}

if ((-s "userhook0") != $offset) {
    die "userhook0 length != $offset";
}



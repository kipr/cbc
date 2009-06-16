#!/usr/bin/perl
#
#  align.pl - aligns images to a 512 byte boundary
#
#  Ken Steele
#  Copyright (c) Chumby Industries, 2007
#
#  align.pl is free software; you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation; either version 2 of the License, or
#  (at your option) any later version.
#
#  align.pl is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with align.pl; if not, write to the Free Software
#  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA

my $filename = $ARGV[0] || die( "syntax: $0 <filename>\n" );
my $filesize = -s $filename;

if( $filesize % 512 )
{
        my $blocksNeeded = int( ( $filesize + 511 ) / 512 );
        print "filesize: $filesize\n";
        print "blocks needed: $blocksNeeded\n";
        my $padding = ( $blocksNeeded * 512 ) - $filesize;
        print "adding $padding bytes of 0xFF padding to $filename\n";

        open( F, ">>$filename" );
        binmode F;

        for( my $i = 0; $i < $padding; ++$i )
        {
                print F "\xff";
        }
        close( F );

        my $newFilesize = -s $filename;
        print "new filesize: $newFilesize\n";
}
else
{
        print "$filename is 512 byte aligned\n";
}


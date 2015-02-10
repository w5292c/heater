#!/usr/bin/perl -w

# The MIT License (MIT)
#
# Copyright (c) 2015 Alexander Chumakov
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

# This script formats the font information in the following format:
#    /** Code: <char-encoding>; Name: <char-name> */
#    0xXX, 0xXX, 0xXX, 0xXX, 0xXX, 0xXX, 0xXX, 0xXX, 0xXX, 0xXX, 0xXX, 0xXX, 0xXX, 0xXX,

$FILENAME='cyr14-etl.bdf';

open CONFIG, $FILENAME;

@face = ();
$encoding = "";
$char_name = "";
$match = 0;
$bitmap = 0;
while (<CONFIG>) {
	$ln = $_;
	chomp ($ln);
	if ($match) {
		if ($ln =~ /ENDCHAR/) {
			&format_char ($encoding, $char_name);
			$match = 0;
			$bitmap = 0;
		}
		elsif ($bitmap) {
			push (@face, $ln);
		}
		elsif ($ln =~ /ENCODING/) {
			$encoding = `expr match "$ln" 'ENCODING \\(.*\\)'`;
		}
		elsif ($ln =~ /BITMAP/) {
			$bitmap = 1;
		}
	}
	elsif ($ln =~ /STARTCHAR/) {
		$match = 1;
		$bitmap = 0;
		$char_name = `expr match "$ln" 'STARTCHAR \\(.*\\)'`;
		$encoding = "";
		@face = ();
	}
}

sub format_char {
	$enc = $_[0];
	$name = $_[1];
	$lx = "";

	chomp ($enc);
	chomp ($name);

	printf ("\t/** Character code: %d; character name: <%s> */\n\t", $enc, $name);

	foreach $line (@face) {
		$lx = $lx . `printf "0x%s, " $line`;
	}
	$_ = $lx;
	s/\s+$//;
	$lx = $_;

	printf ("%s\n", $lx);
}

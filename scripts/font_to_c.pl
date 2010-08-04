#!/usr/bin/perl -w

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

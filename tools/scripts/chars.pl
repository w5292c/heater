#!/usr/bin/perl -w

for ($i = 32; $i < 128; $i++) {
    printf ("Char %3.3d: [%c]\n", $i, $i);
}

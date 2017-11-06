#! /s/std/bin/perl

$pat = @ARGV[0];

while ($line = <STDIN>) {
    $line =~ s/MYAPP/$pat/g;
    printf $line;
}

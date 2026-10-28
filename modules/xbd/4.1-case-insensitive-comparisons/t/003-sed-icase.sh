# sed s///i: the counterpart rule holds for literals, ranges, classes and
# back-references.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XBD.4.1-case-insensitive-comparisons.003
. "$PC_CORE/pc.sh"
pc_begin XBD.4.1-case-insensitive-comparisons.003
LC_ALL=C; export LC_ALL
pc_test

# sub PATTERN LINE EXPECTED: s/PATTERN/X/i applied to LINE gives EXPECTED
sub() {
	got=$(printf '%s\n' "$2" | sed "s/$1/X/i" 2>&1)
	[ "$got" = "$3" ] || pc_fail "s/$1/X/i on '$2' gave '$got', expected '$3'"
}
subn() {
	got=$(printf '%s\n' "$2" | sed "s/$1/X/" 2>&1)
	[ "$got" = "$3" ] || pc_fail "s/$1/X/ on '$2' gave '$got', expected '$3'"
}
subn 'abc' 'AbC' 'AbC'
sub 'abc' 'AbC' 'X'
sub 'ABC' 'aBc' 'X'
sub 'abc' 'abd' 'abd'
sub '[a-c]x' 'Bx' 'X'
sub '[A-C]x' 'cx' 'X'
sub '[[:lower:]]y' 'Qy' 'X'
sub '[[:upper:]]y' 'qy' 'X'
sub '\(a\)\1' 'aA' 'X'
sub '\(a\)\1' 'Aa' 'X'
pc_pass

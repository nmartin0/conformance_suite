# grep -i: a string that matches case-sensitively still matches with any
# of its characters in the other case (literals, bracket ranges, classes,
# back-references, extended syntax).
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XBD.4.1-case-insensitive-comparisons.001
. "$PC_CORE/pc.sh"
pc_begin XBD.4.1-case-insensitive-comparisons.001
LC_ALL=C; export LC_ALL
pc_test

# yes OPTS PATTERN LINE: grep selects LINE
yes() {
	opts=$1; pat=$2; line=$3
	printf '%s\n' "$line" | grep $opts -e "$pat" >/dev/null 2>&1
	rc=$?
	[ "$rc" -eq 0 ] || pc_fail "grep $opts '$pat' did not select '$line' (status $rc)"
}
# no OPTS PATTERN LINE: grep exits 1, selecting nothing
no() {
	opts=$1; pat=$2; line=$3
	printf '%s\n' "$line" | grep $opts -e "$pat" >/dev/null 2>&1
	rc=$?
	[ "$rc" -eq 1 ] || pc_fail "grep $opts '$pat' on '$line' gave status $rc, expected 1"
}

# the same strings do not match when case matters
no -q 'abc' 'AbC'
no -q '[a-c]x' 'Bx'
no -q '[[:lower:]]y' 'Qy'
no -q '\(a\)\1' 'aA'

# literals
yes -i 'abc' 'AbC'
yes -i 'ABC' 'aBc'
no  -i 'abc' 'abd'
# bracket range
yes -i '[a-c]x' 'Bx'
yes -i '[A-C]x' 'cx'
yes -i 'x[b-d]z' 'xCZ'
# classes
yes -i '[[:lower:]]y' 'Qy'
yes -i '[[:upper:]]y' 'qy'
# back-reference: both characters may differ in case
yes -i '\(a\)\1' 'aA'
yes -i '\(a\)\1' 'Aa'
yes -i '\(ab\)\1' 'aBAb'
# extended syntax
yes '-i -E' 'a(b|c)+d' 'AcBD'
yes '-i -E' '^[x-z]{2}$' 'Yz'
no  '-i -E' 'a(b|c)+d' 'aed'
pc_pass

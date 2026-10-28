# The XSI options -b, -c, -d, -o, -s and -x are the same as -t o1, c, u2, o2, d2 and x2.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.od.010
. "$PC_CORE/pc.sh"
pc_begin XCU.od.010
LC_ALL=POSIX
export LC_ALL
NL='
'
printf 'aabbccdd\n' >f1 || pc_fail "cannot create input"
pc_test
for pair in "-b:-t o1" "-c:-t c" "-d:-t u2" "-o:-t o2" "-s:-t d2" "-x:-t x2"; do
	short=${pair%%:*}
	long=${pair#*:}
	od -A n -v $short f1 >out1 2>/dev/null </dev/null
	rc=$?
	[ "$rc" -eq 0 ] || pc_fail "od $short gave status $rc"
	od -A n -v $long f1 >out2 2>/dev/null </dev/null
	a=$(cat out1; printf x)
	b=$(cat out2; printf x)
	if [ "$short" = -c ]; then
		# the page no longer states -c as -t c: compare the fields only
		set -- $a
		a=$*
		set -- $b
		b=$*
	fi
	[ "$a" = "$b" ] || pc_fail "od $short differs from od $long"
	[ "$a" != x ] || pc_fail "od $short wrote nothing"
done
pc_pass

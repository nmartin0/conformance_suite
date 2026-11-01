# The XSI options -b, -c, -d, -o, -s and -x are the same as -t o1, c, u2, o2, d2 and x2.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.od.010
# MUTANT    od.lb_x1
# MUTANT    od.lc_a
# MUTANT    od.ld_d2
# MUTANT    od.ls_u2
# MUTANT    od.lo_o1
# MUTANT    od.lx_x1
# MUTANT    od.l_ignored
# MUTANT    od.l_reversed
# MUTANT    od.l_last_only
# MUTANT    od.l_sorted
# VARIANT   od.gap3
# VARIANT   od.trail_blank
# VARIANT   od.an_empty
# VARIANT   od.pad_none
# VARIANT   od.endian_swap
# VARIANT   od.hex_upper
. "$PC_CORE/pc.sh"
pc_begin XCU.od.010
LC_ALL=POSIX
export LC_ALL
NL='
'
printf 'aabbccdd\n\377\377\200\200\000\377' >f1 || pc_fail "cannot create input"
printf 'aabbccdd\n' >fc || pc_fail "cannot create input"
pc_test
for pair in "-b:-t o1" "-d:-t u2" "-o:-t o2" "-s:-t d2" "-x:-t x2" "-c:-t c"; do
	short=${pair%%:*}
	long=${pair#*:}
	input=f1
	[ "$short" = -c ] && input=fc
	od -A n -v $short $input >out1 2>/dev/null </dev/null
	rc=$?
	[ "$rc" -eq 0 ] || pc_fail "od $short gave status $rc"
	od -A n -v $long $input >out2 2>/dev/null </dev/null
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
# the options for the letters in the presence of each other: the order in
# which the types are given is the order of the lines
cmpopts() {
	od -A n -v $1 f1 >out1 2>/dev/null </dev/null
	rc=$?
	[ "$rc" -eq 0 ] || pc_fail "od $1 gave status $rc"
	od -A n -v $2 f1 >out2 2>/dev/null </dev/null
	a=$(cat out1; printf x)
	b=$(cat out2; printf x)
	[ "$a" = "$b" ] || pc_fail "od $1 differs from od $2"
	[ "$a" != x ] || pc_fail "od $1 wrote nothing"
}
cmpopts "-b -x" "-t o1 -t x2"
cmpopts "-x -b" "-t x2 -t o1"
cmpopts "-bx" "-t o1 -t x2"
cmpopts "-xb" "-t x2 -t o1"
cmpopts "-d -s -o" "-t u2 -t d2 -t o2"
cmpopts "-o -s -d" "-t o2 -t d2 -t u2"
cmpopts "-t x1 -b" "-t x1 -t o1"
cmpopts "-b -t x1" "-t o1 -t x1"
pc_pass

# diff -e writes an ed script, from the end of the file to the start, using the a, c and d commands with text ended by a line holding a period.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.diff.005
# MUTANT    diff.e_c_as_a
# MUTANT    diff.e_cmd_first
# MUTANT    diff.e_d_as_c
# MUTANT    diff.e_no_dot
# MUTANT    diff.e_num_off
# MUTANT    diff.e_order_fwd
# MUTANT    diff.e_space_range
# MUTANT    diff.e_text_dropped
# MUTANT    diff.e_text_prefixed
# VARIANT   diff.e_insert
. "$PC_CORE/pc.sh"
pc_begin XCU.diff.005
LC_ALL=POSIX
export LC_ALL
NL='
'
mkf() {
	n=$1
	shift
	: >"$n" || pc_unresolved "cannot create input"
	for l in "$@"; do printf '%s\n' "$l" >>"$n"; done
}
pc_test
mkf f1 1 2 3 4 5 6 7 8 9
mkf f2 1 X 3 4 5 6 7 Y 9
got=$(diff -e f1 f2 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "8c${NL}Y${NL}.${NL}2c${NL}X${NL}.${NL}rc=1" ] || pc_fail "-e did not give the changes from the end of the file to the start"
mkf g1 a b c
mkf g2 a b x c
got=$(diff -e g1 g2 2>/dev/null </dev/null)
# the text allows the i (insert) command as well as a (append): add after
# line 2 is the same as insert before line 3
case $got in
"2a${NL}x${NL}." | "3i${NL}x${NL}.") ;;
*) pc_fail "-e for an added line did not give 2a x . (or the equivalent 3i x .)" ;;
esac
mkf h1 a b c
mkf h2 a c
got=$(diff -e h1 h2 2>/dev/null </dev/null)
[ "$got" = "2d" ] || pc_fail "-e for a deleted line did not give 2d"
mkf i1 1 2 3 4 5 6
mkf i2 1 4 5 6
got=$(diff -e i1 i2 2>/dev/null </dev/null)
[ "$got" = "2,3d" ] || pc_fail "-e for two deleted lines did not give 2,3d"
mkf j1 a b c d
mkf j2 a X Y Z d
got=$(diff -e j1 j2 2>/dev/null </dev/null)
[ "$got" = "2,3c${NL}X${NL}Y${NL}Z${NL}." ] || pc_fail "-e for a change of 2 lines to 3 did not give 2,3c X Y Z ."
got=$(diff -e f1 f1 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "rc=0" ] || pc_fail "-e with identical files wrote output or gave a status other than 0"
# The script, with a w command appended, must turn file1 into file2 when
# given to ed; this also covers text lines that hold a single period.
if command -v ed >/dev/null 2>&1; then
	rt() { # rt NAME: file NAME.1 -> NAME.2 by the script of diff -e
		diff -e "$1.1" "$1.2" >"$1.sc" 2>/dev/null </dev/null
		[ $? -le 1 ] || pc_fail "round trip $1: diff -e failed"
		cp "$1.1" "$1.w" || pc_unresolved "cannot copy"
		{ cat "$1.sc"; printf 'w\nq\n'; } | ed -s "$1.w" >/dev/null 2>&1
		cmp -s "$1.w" "$1.2" || pc_fail "round trip $1: the -e script did not turn file1 into file2"
	}
	printf 'a\nb\nc\nd\ne\n' >r1.1; printf 'a\n.\nc\nd\ne\n' >r1.2
	rt r1
	printf 'a\nb\n' >r2.1; printf 'a\n.\n.\nb\n' >r2.2
	rt r2
	printf 'a\n.\nb\nc\n' >r3.1; printf 'a\nb\nQ\nc\n' >r3.2
	rt r3
	printf '1\n2\n3\n4\n5\n6\n7\n8\n' >r4.1; printf '1\nX\n3\n5\n6\nY\nZ\n8\nW\n' >r4.2
	rt r4
fi
pc_pass

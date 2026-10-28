# diff -e writes an ed script, from the end of the file to the start, using the a, c and d commands with text ended by a line holding a period.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.diff.005
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
[ "$got" = "2a${NL}x${NL}." ] || pc_fail "-e for an added line did not give 2a x ."
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
pc_pass

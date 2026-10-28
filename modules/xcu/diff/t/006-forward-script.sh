# diff -f writes a script like -e but from the start of the file to the end, with the command letter first and a space between the two numbers of a range.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.diff.006
. "$PC_CORE/pc.sh"
pc_begin XCU.diff.006
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
got=$(diff -f f1 f2 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "c2${NL}X${NL}.${NL}c8${NL}Y${NL}.${NL}rc=1" ] || pc_fail "-f did not give the changes from the start of the file to the end with the letter first"
mkf g1 a b c
mkf g2 a b x c
got=$(diff -f g1 g2 2>/dev/null </dev/null)
[ "$got" = "a2${NL}x${NL}." ] || pc_fail "-f for an added line did not give a2 x ."
mkf h1 a b c
mkf h2 a c
got=$(diff -f h1 h2 2>/dev/null </dev/null)
[ "$got" = "d2" ] || pc_fail "-f for a deleted line did not give d2"
mkf i1 1 2 3 4 5 6
mkf i2 1 4 5 6
got=$(diff -f i1 i2 2>/dev/null </dev/null)
[ "$got" = "d2 3" ] || pc_fail "-f for two deleted lines did not give d2 3"
mkf j1 a b c d
mkf j2 a X Y Z d
got=$(diff -f j1 j2 2>/dev/null </dev/null)
[ "$got" = "c2 3${NL}X${NL}Y${NL}Z${NL}." ] || pc_fail "-f for a change of 2 lines to 3 did not give c2 3 X Y Z ."
pc_pass

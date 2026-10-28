# diff without options writes ed-like lines (NaM, NdM, NcM with ranges) followed by < lines of file1 and > lines of file2, separated by --- for a change.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.diff.001
. "$PC_CORE/pc.sh"
pc_begin XCU.diff.001
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
mkf f1 a b c d e
mkf f2 a b X d e
got=$(diff f1 f2 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "3c3${NL}< c${NL}---${NL}> X${NL}rc=1" ] || pc_fail "a one-line change did not give 3c3 < c --- > X"
mkf f1 a b c
mkf f2 a b x c
got=$(diff f1 f2 2>/dev/null </dev/null)
[ "$got" = "2a3${NL}> x" ] || pc_fail "one added line did not give 2a3 > x"
mkf f1 a b c
mkf f2 a c
got=$(diff f1 f2 2>/dev/null </dev/null)
[ "$got" = "2d1${NL}< b" ] || pc_fail "one deleted line did not give 2d1 < b"
mkf f1 1 2 3 4 5 6
mkf f2 1 4 5 6
got=$(diff f1 f2 2>/dev/null </dev/null)
[ "$got" = "2,3d1${NL}< 2${NL}< 3" ] || pc_fail "two deleted lines did not give 2,3d1"
mkf f1 1 2
mkf f2 1 x y 2
got=$(diff f1 f2 2>/dev/null </dev/null)
[ "$got" = "1a2,3${NL}> x${NL}> y" ] || pc_fail "two added lines did not give 1a2,3"
mkf f1 a b c d
mkf f2 a X Y Z d
got=$(diff f1 f2 2>/dev/null </dev/null)
[ "$got" = "2,3c2,4${NL}< b${NL}< c${NL}---${NL}> X${NL}> Y${NL}> Z" ] || pc_fail "a change of 2 lines to 3 did not give 2,3c2,4"
mkf f1 a b c d
mkf f2 a X d
got=$(diff f1 f2 2>/dev/null </dev/null)
[ "$got" = "2,3c2${NL}< b${NL}< c${NL}---${NL}> X" ] || pc_fail "a change of 2 lines to 1 did not give 2,3c2"
mkf f1 1 2 3 4 5 6 7 8 9
mkf f2 1 X 3 4 5 6 7 Y 9
got=$(diff f1 f2 2>/dev/null </dev/null)
[ "$got" = "2c2${NL}< 2${NL}---${NL}> X${NL}8c8${NL}< 8${NL}---${NL}> Y" ] || pc_fail "two separate changes did not give two hunks in file order"
mkf f1 b c
mkf f2 a b c
got=$(diff f1 f2 2>/dev/null </dev/null)
[ "$got" = "0a1${NL}> a" ] || pc_fail "a line added at the start did not give 0a1"
mkf f1 a b c
mkf f2 a b
got=$(diff f1 f2 2>/dev/null </dev/null)
[ "$got" = "3d2${NL}< c" ] || pc_fail "a line deleted at the end did not give 3d2"
mkf f1 a b
mkf f2 a b c
got=$(diff f1 f2 2>/dev/null </dev/null)
[ "$got" = "2a3${NL}> c" ] || pc_fail "a line added at the end did not give 2a3"
mkf f1 one
mkf f2 two
got=$(diff f1 f2 2>/dev/null </dev/null)
[ "$got" = "1c1${NL}< one${NL}---${NL}> two" ] || pc_fail "a single-line file change did not give 1c1"
pc_pass

# A file operand of - stands for the standard input, in either position.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.diff.009
. "$PC_CORE/pc.sh"
pc_begin XCU.diff.009
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
mkf f1 a b c
mkf f2 a X c
mkf f3 a b c
pc_test
got=$(diff - f2 2>/dev/null <f1; printf 'rc=%s' "$?")
[ "$got" = "2c2${NL}< b${NL}---${NL}> X${NL}rc=1" ] || pc_fail "standard input as file1 wrong"
got=$(diff f1 - 2>/dev/null <f2; printf 'rc=%s' "$?")
[ "$got" = "2c2${NL}< b${NL}---${NL}> X${NL}rc=1" ] || pc_fail "standard input as file2 wrong"
got=$(diff - f3 2>/dev/null <f1; printf 'rc=%s' "$?")
[ "$got" = "rc=0" ] || pc_fail "identical input from standard input gave output or a status other than 0"
got=$(cat f2 | diff f1 - 2>/dev/null; printf 'rc=%s' "$?")
[ "$got" = "2c2${NL}< b${NL}---${NL}> X${NL}rc=1" ] || pc_fail "standard input from a pipe wrong"
got=$(diff f1 f2 2>/dev/null <f3; printf 'rc=%s' "$?")
[ "$got" = "2c2${NL}< b${NL}---${NL}> X${NL}rc=1" ] || pc_fail "two file operands were affected by standard input"
pc_pass

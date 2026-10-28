# fold folds each file operand in order, reads standard input with no operand, and gives a nonzero status for an unreadable file.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.fold.006
. "$PC_CORE/pc.sh"
pc_begin XCU.fold.006
LC_ALL=POSIX
export LC_ALL
NL='
'
printf 'abcdef\n' >f1 && printf 'ghijkl\n' >f2 || pc_fail "cannot create input"
[ ! -e nosuchfile ] || pc_unresolved "nosuchfile exists"
pc_test
got=$(fold -w 4 f1 f2 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "abcd${NL}ef${NL}ghij${NL}kl${NL}rc=0" ] || pc_fail "two files were not folded in order"
got=$(fold -w 4 <f1 2>/dev/null; printf 'rc=%s' "$?")
[ "$got" = "abcd${NL}ef${NL}rc=0" ] || pc_fail "standard input was not folded"
got=$(fold -w 4 nosuchfile f2 2>/dev/null </dev/null; printf 'rc=%s' "$?")
case $got in
*"ghij${NL}kl${NL}rc="[1-9]*) ;;
*) pc_fail "the readable file was not folded, or the status was 0" ;;
esac
pc_pass

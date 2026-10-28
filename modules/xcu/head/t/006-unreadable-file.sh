# A file operand that cannot be read gives a diagnostic and a nonzero status, and the other operands are still handled.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.head.006
. "$PC_CORE/pc.sh"
pc_begin XCU.head.006
LC_ALL=POSIX
export LC_ALL
NL='
'
printf '1\n2\n' >f1 || pc_fail "cannot create input"
[ ! -e nosuchfile ] || pc_unresolved "nosuchfile exists"
pc_test
got=$(head -n 1 nosuchfile f1 2>/dev/null </dev/null; printf 'rc=%s' "$?")
case $got in
*"==> f1 <==${NL}1${NL}rc="[1-9]*) ;;
*) pc_fail "the readable file was not handled, or the status was 0" ;;
esac
pc_pass

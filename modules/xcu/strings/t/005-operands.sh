# strings reads each file operand in turn, standard input when there is no operand, and exits nonzero for a file it cannot read.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.strings.005
. "$PC_CORE/pc.sh"
pc_begin XCU.strings.005
LC_ALL=POSIX
export LC_ALL
NL='
'
printf 'first\000' >f1 && printf 'second\000' >f2 || pc_fail "cannot create input"
[ ! -e nosuchfile ] || pc_unresolved "nosuchfile exists"
pc_test
got=$(strings -a f1 f2 2>/dev/null </dev/null | while IFS= read -r l; do printf '[%s]' "$l"; done)
case $got in
*"[first]"*"[second]"*) ;;
*) pc_fail "the strings of two files were not both written in order" ;;
esac
got=$(strings -a <f2 2>/dev/null | while IFS= read -r l; do printf '[%s]' "$l"; done)
case $got in
*"[second]"*) ;;
*) pc_fail "standard input was not read" ;;
esac
strings -a nosuchfile >out 2>/dev/null </dev/null
rc=$?
[ "$rc" -ne 0 ] || pc_fail "status 0 for a file that cannot be read"
pc_pass

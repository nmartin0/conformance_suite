# alias with no operands writes every definition.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.alias.004
. "$PC_CORE/pc.sh"
pc_begin XCU.alias.004
pc_test
alias pcal4a='echo 1'
alias pcal4b='echo 2'
alias >out 2>/dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "alias with no operands exited $rc"
a= b=
while IFS= read -r line; do
	case $line in
	pcal4a=*) a=1 ;;
	pcal4b=*) b=1 ;;
	esac
done <out
[ -n "$a" ] || pc_fail "pcal4a not listed"
[ -n "$b" ] || pc_fail "pcal4b not listed"
pc_pass

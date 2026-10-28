# command -v writes a line carrying an alias's name and value.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.command.006
. "$PC_CORE/pc.sh"
pc_begin XCU.command.006
alias pcal6='echo six'
pc_test
out=$(command -v pcal6 2>/dev/null)
rc=$?
[ "$rc" -eq 0 ] || pc_fail "command -v for an alias exited $rc"
[ -n "$out" ] || pc_fail "command -v wrote nothing for an alias"
# the page does not fix the line's format: it must carry the name and the value
case $out in
*pcal6*) ;;
*) pc_fail "command -v for an alias wrote '$out', without the name" ;;
esac
case $out in
*'echo six'*) ;;
*) pc_fail "command -v for an alias wrote '$out', without the value" ;;
esac
pc_pass

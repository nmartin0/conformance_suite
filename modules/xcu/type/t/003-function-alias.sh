# type for a function and for an alias succeeds and writes something.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.type.003
. "$PC_CORE/pc.sh"
pc_begin XCU.type.003
pcfn() { :; }
alias pcal3='echo three'
pc_test
type pcfn >out 2>/dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "type of a function exited $rc"
[ -s out ] || pc_fail "type of a function wrote nothing"
type pcal3 >out 2>/dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "type of an alias exited $rc"
[ -s out ] || pc_fail "type of an alias wrote nothing"
pc_pass

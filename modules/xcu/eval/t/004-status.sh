# The exit status of eval is that of the command it executes.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.eval.004
. "$PC_CORE/pc.sh"
pc_begin XCU.eval.004
pc_test
eval true
rc=$?
[ "$rc" -eq 0 ] || pc_fail "eval true gave $rc"
eval false
rc=$?
[ "$rc" -eq 1 ] || pc_fail "eval false gave $rc, expected 1"
eval '(exit 7)'
rc=$?
[ "$rc" -eq 7 ] || pc_fail "eval '(exit 7)' gave $rc, expected 7"
eval 'pcv=1; (exit 3)'
rc=$?
[ "$rc" -eq 3 ] || pc_fail "eval with two commands gave $rc, expected 3"
pc_pass

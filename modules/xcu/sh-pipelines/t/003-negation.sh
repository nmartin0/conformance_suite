# A pipeline that begins with ! has the logical negation of the status it
# would otherwise have: 0 if that was non-zero, else 1.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.sh-pipelines.003
. "$PC_CORE/pc.sh"
pc_begin XCU.sh-pipelines.003
pc_test
! true
rc=$?
[ "$rc" -eq 1 ] || pc_fail "! true gave $rc"
! false
rc=$?
[ "$rc" -eq 0 ] || pc_fail "! false gave $rc"
! (exit 7)
rc=$?
[ "$rc" -eq 0 ] || pc_fail "! (exit 7) gave $rc"
! false | true
rc=$?
[ "$rc" -eq 1 ] || pc_fail "! false | true gave $rc, expected 1"
! true | false
rc=$?
[ "$rc" -eq 0 ] || pc_fail "! true | false gave $rc, expected 0"
pc_pass

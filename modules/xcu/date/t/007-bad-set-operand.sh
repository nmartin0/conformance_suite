# A set operand with month 13 is an error.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.date.007
# MUTANT    date.set_accepts
# MUTANT    date.set_diag_status0
# VARIANT   date.set_status2
# VARIANT   date.set_silent
. "$PC_CORE/pc.sh"
pc_begin XCU.date.007
LC_ALL=C; export LC_ALL
pc_test
date 13999999 >out 2>err
rc=$?
[ "$rc" -gt 0 ] || pc_fail "date 13999999 exited with status $rc"
pc_pass

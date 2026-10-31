# Fixture: pcfs_answer is 42.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.pcfs.002
# MUTANT    pcfs.only_b
. "$PC_CORE/pc.sh"
pc_begin XCU.pcfs.002
[ "${PC_FX_WHICH:-}" != a ] || pc_unresolved "fixture: shell a lacks the facility"
pcfs_answer() { echo 42; }
pc_test
[ "$(pcfs_answer)" = 42 ] || pc_fail "pcfs_answer is not 42"
pc_pass

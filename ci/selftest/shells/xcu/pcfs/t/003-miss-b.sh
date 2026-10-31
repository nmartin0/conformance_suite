# Fixture: pcfs_answer is 42.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.pcfs.003
# MUTANT    pcfs.miss_b
. "$PC_CORE/pc.sh"
pc_begin XCU.pcfs.003
pcfs_answer() { echo 42; }
pc_test
[ "$(pcfs_answer)" = 42 ] || pc_fail "pcfs_answer is not 42"
pc_pass

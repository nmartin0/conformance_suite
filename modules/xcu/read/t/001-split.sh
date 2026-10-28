# read splits a line into the variables named.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.read.001
. "$PC_CORE/pc.sh"
pc_begin XCU.read.001
printf 'one   two  three \n' >in || pc_fail "cannot write the input"
pc_test
read pca pcb <in
rc=$?
[ "$rc" -eq 0 ] || pc_fail "read exited $rc"
[ "$pca" = one ] || pc_fail "first variable is '$pca', wanted one"
[ "$pcb" = 'two  three' ] || pc_fail "second variable is '$pcb', wanted 'two  three'"
pc_pass

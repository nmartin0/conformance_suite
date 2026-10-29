# read in a subshell leaves the parent's variables.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.read.012
. "$PC_CORE/pc.sh"
pc_begin XCU.read.012
printf 'new\n' >in || pc_fail "cannot write the input"
pcsub=orig
pc_test
( read pcsub && [ "$pcsub" = new ] ) <in || pc_fail "read did not set the variable inside the subshell"
[ "$pcsub" = orig ] || pc_fail "variable changed to '$pcsub' by a subshell"
pc_pass

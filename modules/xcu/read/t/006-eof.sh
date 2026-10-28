# End of input without a newline sets the variable and exits 1.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.read.006
. "$PC_CORE/pc.sh"
pc_begin XCU.read.006
printf abc >in1 || pc_fail "cannot write the input"
: >in2 || pc_fail "cannot write the input"
pca=old
pc_test
read pca <in1
rc=$?
[ "$rc" -eq 1 ] || pc_fail "partial line: exit $rc, wanted 1"
[ "$pca" = abc ] || pc_fail "partial line: variable is '$pca', wanted abc"
pca=old
read pca <in2
rc=$?
[ "$rc" -eq 1 ] || pc_fail "empty input: exit $rc, wanted 1"
[ "${pca+set}" = set ] && [ -z "$pca" ] || pc_fail "empty input: variable is '${pca-unset}', wanted empty"
pc_pass

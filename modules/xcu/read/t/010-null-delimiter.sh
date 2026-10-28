# read -d with the empty string uses the null byte.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.read.010
. "$PC_CORE/pc.sh"
pc_begin XCU.read.010
printf 'ab\000cd' >in || pc_fail "cannot write the input"
pc_test
read -d '' pca <in
rc=$?
[ "$rc" -eq 0 ] || pc_fail "read -d '' exited $rc"
[ "$pca" = ab ] || pc_fail "variable is '$pca', wanted ab"
pc_pass

# With one variable the line is kept, trimmed at the ends.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.read.002
. "$PC_CORE/pc.sh"
pc_begin XCU.read.002
printf '  lead  mid trail  \n' >in || pc_fail "cannot write the input"
pc_test
read pca <in
rc=$?
[ "$rc" -eq 0 ] || pc_fail "read exited $rc"
[ "$pca" = 'lead  mid trail' ] || pc_fail "variable is '$pca', wanted 'lead  mid trail'"
pc_pass

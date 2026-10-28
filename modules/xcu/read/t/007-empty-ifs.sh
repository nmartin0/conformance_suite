# With IFS empty the first variable gets the whole line.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.read.007
. "$PC_CORE/pc.sh"
pc_begin XCU.read.007
printf '  x y  \n' >in || pc_fail "cannot write the input"
pcb=old
pc_test
IFS= read -r pca pcb <in
rc=$?
[ "$rc" -eq 0 ] || pc_fail "read exited $rc"
[ "$pca" = '  x y  ' ] || pc_fail "first variable is '$pca', wanted the whole line"
[ "${pcb+set}" = set ] && [ -z "$pcb" ] || pc_fail "second variable is '${pcb-unset}', wanted empty"
pc_pass

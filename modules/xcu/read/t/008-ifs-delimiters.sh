# A non-blank IFS character separates fields and empty fields count.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.read.008
. "$PC_CORE/pc.sh"
pc_begin XCU.read.008
printf 'x::z:w\n' >in || pc_fail "cannot write the input"
pc_test
IFS=: read pca pcb pcc <in
rc=$?
[ "$rc" -eq 0 ] || pc_fail "read exited $rc"
[ "$pca" = x ] || pc_fail "first field is '$pca', wanted x"
[ "${pcb+set}" = set ] && [ -z "$pcb" ] || pc_fail "second field is '${pcb-unset}', wanted empty"
[ "$pcc" = 'z:w' ] || pc_fail "last variable is '$pcc', wanted 'z:w'"
pc_pass

# Variables without a field are set empty.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.read.003
. "$PC_CORE/pc.sh"
pc_begin XCU.read.003
printf 'x y\n' >in || pc_fail "cannot write the input"
pca=old pcb=old pcc=old pcd=old
pc_test
read pca pcb pcc pcd <in
rc=$?
[ "$rc" -eq 0 ] || pc_fail "read exited $rc"
[ "$pca" = x ] && [ "$pcb" = y ] || pc_fail "fields are '$pca' and '$pcb'"
[ "${pcc+set}" = set ] && [ -z "$pcc" ] || pc_fail "third variable is '${pcc-unset}', wanted empty"
[ "${pcd+set}" = set ] && [ -z "$pcd" ] || pc_fail "fourth variable is '${pcd-unset}', wanted empty"
pc_pass

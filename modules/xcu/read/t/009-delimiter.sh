# read -d takes another byte as the end of the line.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.read.009
. "$PC_CORE/pc.sh"
pc_begin XCU.read.009
nl='
'
printf 'ab;cd\nx\n' >in1 || pc_fail "cannot write the input"
printf 'x\ny;z' >in2 || pc_fail "cannot write the input"
pc_test
{
	read -d ';' pca
	rc1=$?
	read pcb
	rc2=$?
} <in1
[ "$rc1" -eq 0 ] || pc_fail "read -d exited $rc1"
[ "$pca" = ab ] || pc_fail "read -d gives '$pca', wanted ab"
[ "$rc2" -eq 0 ] && [ "$pcb" = cd ] || pc_fail "the next read gives '$pcb' (exit $rc2), wanted cd"
read -d ';' pca <in2
[ "$pca" = "x${nl}y" ] || pc_fail "a newline in the data was not kept: '$pca'"
pc_pass

# type with several operands covers each and fails if one is unknown.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.type.004
. "$PC_CORE/pc.sh"
pc_begin XCU.type.004
mkdir bin || pc_fail "cannot make a directory"
printf 'exit 0\n' >bin/pcty1 || pc_fail "cannot write a script"
printf 'exit 0\n' >bin/pcty2 || pc_fail "cannot write a script"
chmod +x bin/pcty1 bin/pcty2 || pc_fail "cannot make the scripts executable"
PATH=$PWD/bin:$PATH
type pcty1 >one 2>/dev/null || pc_fail "type pcty1 failed"
pc_test
type pcty1 pcty2 >two 2>/dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "type with two known names exited $rc"
set -- $(wc -c <one)
l1=$1
set -- $(wc -c <two)
l2=$1
[ "$l2" -gt "$l1" ] || pc_fail "the output for two operands ($l2 bytes) is not larger than for one ($l1 bytes)"
type pcty1 pcnosuch9 >/dev/null 2>&1
rc=$?
[ "$rc" -gt 0 ] || pc_fail "type with an unknown operand exited $rc"
pc_pass

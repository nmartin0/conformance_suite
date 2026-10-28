# An empty directory operand is an error.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.cd.012
. "$PC_CORE/pc.sh"
pc_begin XCU.cd.012
unset CDPATH
mkdir d1 || pc_fail "cannot make a directory"
: >d1/m1 || pc_fail "cannot make a marker file"
cd d1 || pc_fail "cd d1 failed"
pc_test
oldpwd=$PWD
cd '' >/dev/null 2>err
rc=$?
[ "$rc" -gt 0 ] || pc_fail "cd '' exited $rc"
[ -s err ] || pc_fail "no diagnostic for an empty operand"
[ -f m1 ] && [ "$PWD" = "$oldpwd" ] || pc_fail "the directory changed after cd ''"
pc_pass

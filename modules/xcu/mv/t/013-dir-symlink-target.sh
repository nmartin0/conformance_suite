# A final operand that is a symbolic link to an existing directory is used as the target directory.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.mv.013
. "$PC_CORE/pc.sh"
pc_begin XCU.mv.013
mkdir d || pc_fail "cannot create d"
ln -s d lnk || pc_fail "cannot create symbolic link"
printf 'x\n' >a || pc_fail "cannot create a"
pc_test
mv a lnk >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "mv exited with status $rc"
[ -f d/a ] || pc_fail "d/a does not exist"
[ -h lnk ] || pc_fail "lnk is no longer a symbolic link"
pc_pass

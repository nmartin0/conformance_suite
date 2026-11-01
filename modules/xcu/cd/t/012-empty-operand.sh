# An empty directory operand is an error.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.cd.012
# MUTANT    cd.empty_status0
# MUTANT    cd.empty_no_diag
# MUTANT    cd.empty_stdout_diag
# MUTANT    cd.empty_as_dot
# MUTANT    cd.empty_as_home
# MUTANT    cd.empty_changes
# MUTANT    cd.empty_cdpath
# VARIANT   cd.status2
# VARIANT   cd.status255
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
CDPATH=$oldpwd
cd '' >/dev/null 2>err
rc=$?
unset CDPATH
[ "$rc" -gt 0 ] || pc_fail "cd '' exited $rc when CDPATH is set"
[ -s err ] || pc_fail "no diagnostic for an empty operand when CDPATH is set"
[ -f m1 ] && [ "$PWD" = "$oldpwd" ] || pc_fail "the directory changed after cd '' with CDPATH set"
pc_pass

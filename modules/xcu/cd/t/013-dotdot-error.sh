# A dot-dot after a component that is not a directory is an error.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.cd.013
# MUTANT    cd.dotdot_missing_ok
# MUTANT    cd.dotdot_file_ok
# MUTANT    cd.dotdot_moves
# MUTANT    cd.dotdot_no_diag
# MUTANT    cd.dotdot_diag_stdout
# VARIANT   cd.status2
# VARIANT   cd.status255
. "$PC_CORE/pc.sh"
pc_begin XCU.cd.013
unset CDPATH
mkdir d1 || pc_fail "cannot make a directory"
: >d1/m1 || pc_fail "cannot make a marker file"
: >d1/afile || pc_fail "cannot make a file"
cd d1 || pc_fail "cd d1 failed"
pc_test
oldpwd=$PWD
cd nosuch/.. >/dev/null 2>err
rc=$?
[ "$rc" -gt 0 ] || pc_fail "cd nosuch/.. exited $rc"
[ -s err ] || pc_fail "no diagnostic for cd nosuch/.."
[ -f m1 ] && [ "$PWD" = "$oldpwd" ] || pc_fail "the directory changed after cd nosuch/.."
cd afile/.. >/dev/null 2>err
rc=$?
[ "$rc" -gt 0 ] || pc_fail "cd afile/.. exited $rc"
[ -s err ] || pc_fail "no diagnostic for cd afile/.."
[ -f m1 ] && [ "$PWD" = "$oldpwd" ] || pc_fail "the directory changed after cd afile/.."
pc_pass

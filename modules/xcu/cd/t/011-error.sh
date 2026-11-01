# cd to a missing directory or a file fails and changes nothing.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.cd.011
# MUTANT    cd.missing_status0
# MUTANT    cd.file_status0
# MUTANT    cd.missing_no_diag
# MUTANT    cd.file_no_diag
# MUTANT    cd.diag_stdout
# MUTANT    cd.error_moves
# MUTANT    cd.error_pwd_changed
# VARIANT   cd.status2
# VARIANT   cd.status255
. "$PC_CORE/pc.sh"
pc_begin XCU.cd.011
unset CDPATH
mkdir d1 || pc_fail "cannot make a directory"
: >d1/m1 || pc_fail "cannot make a marker file"
: >afile || pc_fail "cannot make a file"
base=$PWD
cd d1 || pc_fail "cd d1 failed"
pc_test
oldpwd=$PWD
cd nosuchdir >/dev/null 2>err
rc=$?
[ "$rc" -gt 0 ] || pc_fail "cd to a missing directory exited $rc"
[ -s err ] || pc_fail "no diagnostic for a missing directory"
[ -f m1 ] && [ "$PWD" = "$oldpwd" ] || pc_fail "the directory changed after a failed cd"
cd "$base/afile" >/dev/null 2>err
rc=$?
[ "$rc" -gt 0 ] || pc_fail "cd to a regular file exited $rc"
[ -s err ] || pc_fail "no diagnostic for a regular file"
[ -f m1 ] && [ "$PWD" = "$oldpwd" ] || pc_fail "the directory changed after cd to a file"
pc_pass

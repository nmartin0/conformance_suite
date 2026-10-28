# cd to a missing directory or a file fails and changes nothing.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.cd.011
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
cd "$base/afile" >/dev/null 2>&1
rc=$?
[ "$rc" -gt 0 ] || pc_fail "cd to a regular file exited $rc"
[ -f m1 ] && [ "$PWD" = "$oldpwd" ] || pc_fail "the directory changed after cd to a file"
pc_pass

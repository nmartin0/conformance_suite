# A dot-dot after a component that is not a directory is an error.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.cd.013
. "$PC_CORE/pc.sh"
pc_begin XCU.cd.013
unset CDPATH
mkdir d1 || pc_fail "cannot make a directory"
: >d1/m1 || pc_fail "cannot make a marker file"
: >d1/afile || pc_fail "cannot make a file"
cd d1 || pc_fail "cd d1 failed"
pc_test
oldpwd=$PWD
cd nosuch/.. >/dev/null 2>&1
rc=$?
[ "$rc" -gt 0 ] || pc_fail "cd nosuch/.. exited $rc"
[ -f m1 ] && [ "$PWD" = "$oldpwd" ] || pc_fail "the directory changed after cd nosuch/.."
cd afile/.. >/dev/null 2>&1
rc=$?
[ "$rc" -gt 0 ] || pc_fail "cd afile/.. exited $rc"
[ -f m1 ] && [ "$PWD" = "$oldpwd" ] || pc_fail "the directory changed after cd afile/.."
pc_pass

# pwd -P writes the physical directory: no component that is a symbolic link.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.pwd.002
. "$PC_CORE/pc.sh"
pc_begin XCU.pwd.002
root=$(pwd -P) || pc_fail "cannot determine the directory"
mkdir real || pc_fail "cannot create real"
ln -s real lnk || pc_fail "cannot create symbolic link"
cd "$root/lnk" || pc_fail "cannot enter lnk"
pc_test
out=$(pwd -P)
rc=$?
[ "$rc" -eq 0 ] || pc_fail "pwd -P exited with status $rc"
[ "$out" = "$root/real" ] || pc_fail "pwd -P wrote '$out', expected '$root/real'"
out=$( (exec pwd -P) )
rc=$?
[ "$rc" -eq 0 ] || pc_fail "exec pwd -P exited with status $rc"
[ "$out" = "$root/real" ] || pc_fail "exec pwd -P wrote '$out', expected '$root/real'"
pc_pass

# When both -L and -P are given the last one applies.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.pwd.004
. "$PC_CORE/pc.sh"
pc_begin XCU.pwd.004
root=$(pwd -P) || pc_fail "cannot determine the directory"
mkdir real || pc_fail "cannot create real"
ln -s real lnk || pc_fail "cannot create symbolic link"
cd "$root/lnk" || pc_fail "cannot enter lnk"
pc_test
out=$(pwd -L -P)
[ "$out" = "$root/real" ] || pc_fail "pwd -L -P wrote '$out', expected '$root/real'"
out=$(pwd -P -L)
[ "$out" = "$root/lnk" ] || pc_fail "pwd -P -L wrote '$out', expected '$root/lnk'"
pc_pass

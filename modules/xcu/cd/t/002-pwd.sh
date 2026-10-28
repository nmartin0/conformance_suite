# PWD names the new directory as an absolute pathname.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.cd.002
. "$PC_CORE/pc.sh"
pc_begin XCU.cd.002
unset CDPATH
mkdir d1 d2 || pc_fail "cannot make directories"
base=$PWD
pc_test
cd d1 || pc_fail "cd d1 failed"
[ "$PWD" = "$base/d1" ] || pc_fail "PWD is '$PWD', wanted $base/d1"
cd ../d2 || pc_fail "cd ../d2 failed"
[ "$PWD" = "$base/d2" ] || pc_fail "PWD is '$PWD' after cd ../d2, wanted $base/d2"
cd "$base" || pc_fail "cd to the base failed"
cd d1/../d2 || pc_fail "cd d1/../d2 failed"
[ "$PWD" = "$base/d2" ] || pc_fail "PWD is '$PWD' after cd d1/../d2, wanted $base/d2"
pc_pass

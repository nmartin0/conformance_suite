# cd with no operand goes to HOME.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.cd.005
. "$PC_CORE/pc.sh"
pc_begin XCU.cd.005
unset CDPATH
mkdir d1 || pc_fail "cannot make a directory"
: >d1/m1 || pc_fail "cannot make a marker file"
base=$PWD
out=$base/cdout
pc_test
HOME=$base/d1
export HOME
cd >"$out" 2>/dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "cd exited $rc"
[ -f m1 ] || pc_fail "not in HOME after cd"
[ ! -s "$out" ] || pc_fail "cd with no operand wrote to standard output"
pc_pass

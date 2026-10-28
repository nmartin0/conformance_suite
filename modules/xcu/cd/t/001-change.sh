# cd changes the directory, exits 0 and writes nothing.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.cd.001
. "$PC_CORE/pc.sh"
pc_begin XCU.cd.001
unset CDPATH
mkdir d1 d2 || pc_fail "cannot make directories"
: >d1/m1 && : >d2/m2 || pc_fail "cannot make marker files"
base=$PWD
out=$base/cdout
pc_test
cd d1 >"$out" 2>/dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "cd d1 exited $rc"
[ -f m1 ] || pc_fail "not in d1 after cd d1"
[ ! -s "$out" ] || pc_fail "cd d1 wrote to standard output"
cd "$base/d2" >"$out" 2>/dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "cd with an absolute operand exited $rc"
[ -f m2 ] || pc_fail "not in d2 after cd with an absolute operand"
[ ! -s "$out" ] || pc_fail "cd with an absolute operand wrote to standard output"
pc_pass

# cd - goes back and writes the directory.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.cd.004
. "$PC_CORE/pc.sh"
pc_begin XCU.cd.004
unset CDPATH
nl='
'
mkdir d1 d2 || pc_fail "cannot make directories"
: >d1/m1 || pc_fail "cannot make a marker file"
base=$PWD
out=$base/cdout
pc_test
cd d1 || pc_fail "cd d1 failed"
cd ../d2 || pc_fail "cd ../d2 failed"
cd - >"$out" 2>/dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "cd - exited $rc"
[ -f m1 ] || pc_fail "not back in d1 after cd -"
[ "$PWD" = "$base/d1" ] || pc_fail "PWD is '$PWD' after cd -, wanted $base/d1"
got=$(cat "$out"; echo x)
[ "$got" = "$base/d1${nl}x" ] || pc_fail "cd - did not write the new directory and a newline"
pc_pass

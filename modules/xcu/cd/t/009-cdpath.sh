# CDPATH directories are searched and the result is written.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.cd.009
. "$PC_CORE/pc.sh"
pc_begin XCU.cd.009
nl='
'
mkdir x1 cp cp/sub || pc_fail "cannot make directories"
: >cp/sub/msub || pc_fail "cannot make a marker file"
base=$PWD
out=$base/cdout
pc_test
CDPATH=$base/cp
cd sub >"$out" 2>/dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "cd sub exited $rc"
[ -f msub ] || pc_fail "not in cp/sub after cd sub"
[ "$PWD" = "$base/cp/sub" ] || pc_fail "PWD is '$PWD', wanted $base/cp/sub"
got=$(cat "$out"; echo x)
[ "$got" = "$base/cp/sub${nl}x" ] || pc_fail "cd did not write the directory found through CDPATH"
cd "$base" || pc_fail "cd to the base failed"
CDPATH=$base/x1:$base/cp
cd sub >"$out" 2>/dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "cd sub with two CDPATH entries exited $rc"
[ -f msub ] || pc_fail "not in cp/sub with two CDPATH entries"
pc_pass

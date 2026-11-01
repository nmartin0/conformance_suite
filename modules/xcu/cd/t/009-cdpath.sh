# CDPATH directories are searched and the result is written.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.cd.009
# MUTANT    cd.cdpath_ignored
# MUTANT    cd.cdpath_first_only
# MUTANT    cd.cdpath_reverse
# MUTANT    cd.cdpath_exists_not_dir
# MUTANT    cd.cdpath_silent
# MUTANT    cd.cdpath_stderr
# MUTANT    cd.cdpath_nonl
# MUTANT    cd.cdpath_operand_written
# MUTANT    cd.cdpath_pwd_stale
. "$PC_CORE/pc.sh"
pc_begin XCU.cd.009
nl='
'
mkdir x1 cp cp/sub cp0 ca cb ca/both cb/both || pc_fail "cannot make directories"
: >cp/sub/msub || pc_fail "cannot make a marker file"
: >cp0/sub && : >ca/both/ma && : >cb/both/mb || pc_fail "cannot make marker files"
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
cd "$base" || pc_fail "cd to the base failed"
CDPATH=$base/cp0:$base/cp
cd sub >"$out" 2>/dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "cd sub exited $rc when an earlier CDPATH entry holds a file of that name"
[ -f msub ] || pc_fail "an earlier CDPATH match that is not a directory was not passed over"
cd "$base" || pc_fail "cd to the base failed"
CDPATH=$base/ca:$base/cb
cd both >"$out" 2>/dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "cd both exited $rc"
[ -f ma ] || pc_fail "the CDPATH entries were not searched in order"
cd "$base" || pc_fail "cd to the base failed"
CDPATH=$base/cb:$base/ca
cd both >"$out" 2>/dev/null || pc_fail "cd both with the entries swapped failed"
[ -f mb ] || pc_fail "the CDPATH entries were not searched in order when swapped"
pc_pass

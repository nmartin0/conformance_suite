# An empty CDPATH entry is the current directory; ./ and ../ skip CDPATH.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.cd.010
. "$PC_CORE/pc.sh"
pc_begin XCU.cd.010
mkdir dup cp cp/dup || pc_fail "cannot make directories"
: >dup/mlocal && : >cp/dup/mcp || pc_fail "cannot make marker files"
base=$PWD
out=$base/cdout
pc_test
CDPATH=:$base/cp
cd dup >"$out" 2>/dev/null || pc_fail "cd dup with a leading empty entry failed"
[ -f mlocal ] || pc_fail "a leading empty CDPATH entry did not select the local directory"
[ ! -s "$out" ] || pc_fail "output written when the empty entry was used"
cd "$base" || pc_fail "cd to the base failed"
CDPATH=$base/cp:
cd dup >"$out" 2>/dev/null || pc_fail "cd dup with a trailing empty entry failed"
[ -f mcp ] || pc_fail "the first CDPATH entry was not used before the empty one"
[ -s "$out" ] || pc_fail "nothing written when a CDPATH directory was used"
cd "$base" || pc_fail "cd to the base failed"
CDPATH=$base/cp
cd ./dup >"$out" 2>/dev/null || pc_fail "cd ./dup failed"
[ -f mlocal ] || pc_fail "cd ./dup looked in CDPATH"
[ ! -s "$out" ] || pc_fail "output written for cd ./dup"
cd "$base/cp" || pc_fail "cd to cp failed"
cd ../dup >"$out" 2>/dev/null || pc_fail "cd ../dup failed"
[ -f mlocal ] || pc_fail "cd ../dup looked in CDPATH"
[ ! -s "$out" ] || pc_fail "output written for cd ../dup"
pc_pass

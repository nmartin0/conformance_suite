# An empty CDPATH entry is the current directory; ./ and ../ skip CDPATH.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.cd.010
# MUTANT    cd.empty_skipped
# MUTANT    cd.empty_root
# MUTANT    cd.empty_first
# MUTANT    cd.empty_prints
# MUTANT    cd.dotslash_cdpath
# MUTANT    cd.dotdot_slash_cdpath
# MUTANT    cd.dot_lone_cdpath
# MUTANT    cd.dot_prints
. "$PC_CORE/pc.sh"
pc_begin XCU.cd.010
mkdir dup cp cp/dup q q/cp3 q/dup || pc_fail "cannot make directories"
: >dup/mlocal && : >cp/dup/mcp && : >q/dup/mq && : >mbase || pc_fail "cannot make marker files"
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
CDPATH=$base/q/cp3
cd "$base/cp" || pc_fail "cd to cp failed"
cd ../dup >"$out" 2>/dev/null || pc_fail "cd ../dup failed"
[ -f mlocal ] || pc_fail "cd ../dup looked in CDPATH"
[ ! -s "$out" ] || pc_fail "output written for cd ../dup"
cd "$base/cp" || pc_fail "cd to cp failed"
cd .. >"$out" 2>/dev/null || pc_fail "cd .. failed"
[ -f mbase ] || pc_fail "cd .. looked in CDPATH"
[ ! -s "$out" ] || pc_fail "output written for cd .."
cd "$base/cp" || pc_fail "cd to cp failed"
CDPATH=$base/q
cd . >"$out" 2>/dev/null || pc_fail "cd . failed"
[ "$PWD" = "$base/cp" ] || pc_fail "cd . changed the directory (PWD is '$PWD')"
[ ! -s "$out" ] || pc_fail "output written for cd ."
pc_pass

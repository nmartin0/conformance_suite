# pushdef, popdef, undefine, ifdef, defn
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.m4.005
# pushdef saves a definition that popdef restores; popdef of the last
# definition undefines; undefine removes all definitions; ifdef tests
# definition; defn gives a definition that can define another macro.
. "$PC_CORE/pc.sh"
pc_begin XCU.m4.005
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
cat >in <<'EOM'
define(`x',1)pushdef(`x',2)x popdef(`x')x popdef(`x')x
define(`q',1)ifdef(`q',D,U) ifdef(`zz',D,U) ifdef(`zz',D)
define(`y',`a')pushdef(`y',`b')undefine(`y')ifdef(`y',D,U)
define(`z',`hello')define(`w',defn(`z'))w
EOM
printf '2 1 x\nD U \nU\nhello\n' >exp
pc_test
m4 in >out 2>err </dev/null || pc_fail "m4: nonzero status"
cmp out exp >/dev/null 2>&1 || pc_fail "output differs: $(cat out)"
pc_pass

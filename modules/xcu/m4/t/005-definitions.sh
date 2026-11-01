# pushdef, popdef, undefine, ifdef, defn
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.m4.005
# pushdef saves a definition that popdef restores; popdef of the last
# definition undefines; undefine removes all definitions; ifdef tests
# definition; defn gives a definition that can define another macro.
# MUTANT    m4.pushdef_replaces
# MUTANT    m4.pushdef_ignored
# MUTANT    m4.define_noreplace
# MUTANT    m4.define_pushes
# MUTANT    m4.define_ignored
# MUTANT    m4.define_second_empty
# MUTANT    m4.popdef_all
# MUTANT    m4.popdef_noop
# MUTANT    m4.popdef_first_only
# MUTANT    m4.undefine_top_only
# MUTANT    m4.undefine_noop
# MUTANT    m4.undefine_first_only
# MUTANT    m4.ifdef_inverted
# MUTANT    m4.ifdef_empty_undefined
# MUTANT    m4.ifdef_nobuiltin
# MUTANT    m4.ifdef_always_second
# MUTANT    m4.defn_unquoted
# MUTANT    m4.defn_name
# VARIANT   m4.engine
# VARIANT   m4.define_replaces_stack
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
define(`z',`hello')define(`w',defn(`z'))define(`z',`bye')w
define(`k',`kk')define(`kk',`K')define(`z2',`k')define(`w2',defn(`z2'))undefine(`k')w2
define(`myif',defn(`ifelse'))myif(a,a,yes,no)
define(`p',1)define(`p',2)p popdef(`p')p
define(`e1',)ifdef(`e1',D,U) ifdef(`define',D,U)
define(`u1',1)define(`u2',2)undefine(`u1',`u2')ifdef(`u1',D,U)`'ifdef(`u2',D,U)
pushdef(`s2',1)pushdef(`s2',2)popdef(`s2')s2
pushdef(`m1',1)pushdef(`m1',2)pushdef(`m2',1)pushdef(`m2',2)popdef(`m1',`m2')m1 m2
EOM
printf '2 1 x\nD U \nU\nhello\nk\nyes\n2 p\nD D\nUU\n1\n1 1\n' >exp
cat >in2 <<'EOM'
pushdef(`s',1)pushdef(`s',2)define(`s',3)s popdef(`s')ifdef(`s',s,none)
EOM
pc_test
m4 in >out 2>err </dev/null || pc_fail "m4: nonzero status"
cmp out exp >/dev/null 2>&1 || pc_fail "output differs: $(cat out)"
m4 in2 >out2 2>err </dev/null || pc_fail "define after pushdef: nonzero status"
# it is unspecified whether define changed only the newest definition or
# replaced them all
case $(cat out2) in
'3 1' | '3 none') ;;
*) pc_fail "define after pushdef gave '$(cat out2)'" ;;
esac
pc_pass

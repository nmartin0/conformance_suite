# null values and traceon
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.m4.018
# Built-in macros with no stated value expand to nothing; traceon output goes
# to standard error.
# MUTANT    m4.define_returns_name
# MUTANT    m4.define_returns_text
# MUTANT    m4.pushdef_returns_name
# MUTANT    m4.pushdef_returns_text
# MUTANT    m4.popdef_returns_name
# MUTANT    m4.undefine_returns_name
# MUTANT    m4.ifdef_undef_emits_name
# MUTANT    m4.ifelse_nonequal_third
# MUTANT    m4.divert_returns_num
# MUTANT    m4.m4wrap_returns_text
# MUTANT    m4.errprint_returns_text
# MUTANT    m4.traceon_returns_text
# MUTANT    m4.trace_stdout
# MUTANT    m4.trace_nothing
# MUTANT    m4.traceoff_noop
# MUTANT    m4.dumpdef_stdout
# MUTANT    m4.dumpdef_nothing
# MUTANT    m4.dumpdef_name_only
. "$PC_CORE/pc.sh"
pc_begin XCU.m4.018
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
cat >in <<'EOM'
A[define(`q',`1')pushdef(`q',`2')popdef(`q')undefine(`q')ifdef(`q',`x')ifelse(`a',`b',`c')divert(0)undivert`'m4wrap(`')sinclude(`/nonexistent.pc')errprint(`')traceon(`define')traceoff(`define')undivert(1)popdef(`never')pushdef(`q2',`')syscmd(`:')changequote`'changecom`']B
EOM
cat >in2 <<'EOM'
define(`foo',`x')traceon(`foo')foo
EOM
cat >in3 <<'EOM'
A[define(`pcq',`pcdefxyz')dumpdef(`pcq')]B
EOM
cat >in4 <<'EOM'
define(`foo',`x')traceon(`foo')traceoff(`foo')foo
EOM
printf 'A[]B\n' >exp
printf 'x\n' >exp2
pc_test
m4 in >out 2>err </dev/null || pc_fail "m4 failed"
cmp -s out exp || pc_fail "output was '$(cat out)', expected '$(cat exp)'"
m4 in2 >out2 2>err2 </dev/null || pc_fail "m4 failed with traceon"
cmp -s out2 exp2 || pc_fail "with traceon the output was '$(cat out2)', expected x"
[ -s err2 ] || pc_fail "traceon wrote nothing to standard error"
m4 in3 >out3 2>err3 </dev/null || pc_fail "m4 failed with dumpdef"
cmp -s out3 exp || pc_fail "with dumpdef the output was '$(cat out3)', expected A[]B"
grep pcdefxyz err3 >/dev/null || pc_fail "dumpdef did not write the defined text to standard error"
m4 in4 >out4 2>err4 </dev/null || pc_fail "m4 failed with traceoff"
cmp -s out4 exp2 || pc_fail "with traceoff the output was '$(cat out4)', expected x"
[ ! -s err4 ] || pc_fail "a macro was traced after traceoff"
pc_pass

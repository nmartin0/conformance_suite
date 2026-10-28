# null values and traceon
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.m4.018
# Built-in macros with no stated value expand to nothing; traceon output goes
# to standard error.
. "$PC_CORE/pc.sh"
pc_begin XCU.m4.018
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
cat >in <<'EOM'
A[define(`q',`1')pushdef(`q',`2')popdef(`q')undefine(`q')ifdef(`q',`x')ifelse(`a',`b',`c')divert(0)undivert`'m4wrap(`')sinclude(`/nonexistent.pc')errprint(`')traceon(`define')traceoff(`define')]B
EOM
cat >in2 <<'EOM'
define(`foo',`x')traceon(`foo')foo
EOM
printf 'A[]B\n' >exp
printf 'x\n' >exp2
pc_test
m4 in >out 2>err </dev/null || pc_fail "m4 failed"
cmp -s out exp || pc_fail "output was '$(cat out)', expected '$(cat exp)'"
m4 in2 >out2 2>err2 </dev/null || pc_fail "m4 failed with traceon"
cmp -s out2 exp2 || pc_fail "with traceon the output was '$(cat out2)', expected x"
[ -s err2 ] || pc_fail "traceon wrote nothing to standard error"
pc_pass

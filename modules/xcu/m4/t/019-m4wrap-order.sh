# m4wrap order
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.m4.019
# Text given to m4wrap more than once is processed in the order of the calls.
# MUTANT    m4.m4wrap_reverse
# MUTANT    m4.m4wrap_ignored
. "$PC_CORE/pc.sh"
pc_begin XCU.m4.019
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
cat >in <<'EOM'
m4wrap(`A')m4wrap(`B')dnl
body
EOM
printf 'body\nAB' >exp
cat >in2 <<'EOM'
m4wrap(`A')m4wrap(`B')m4wrap(`C')dnl
body
EOM
printf 'body\nABC' >exp2
pc_test
m4 in >out 2>err </dev/null || pc_fail "m4wrap: nonzero status"
cmp out exp >/dev/null 2>&1 || pc_fail "m4wrap texts were not processed in call order: $(cat out)"
m4 in2 >out2 2>err </dev/null || pc_fail "m4wrap three times: nonzero status"
cmp out2 exp2 >/dev/null 2>&1 || pc_fail "three m4wrap texts were not processed in call order: $(cat out2)"
pc_pass

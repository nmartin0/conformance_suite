# divert, undivert, divnum, m4wrap
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.m4.007
# Diverted text is held and written at the end in buffer order; divert(-1)
# discards; divnum gives the current stream; undivert writes a buffer at
# once and empties it; m4wrap text is processed at end of input in call
# order.
. "$PC_CORE/pc.sh"
pc_begin XCU.m4.007
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
cat >in <<'EOM'
start
divert(-1)dnl
hidden
divert(3)dnl
divnum
divert(2)dnl
two
divert(1)dnl
one
divert(0)dnl
undivert(3)dnl
end
EOM
printf 'start\n3\nend\none\ntwo\n' >exp
cat >in2 <<'EOM'
m4wrap(`A')m4wrap(`B')dnl
body
EOM
printf 'body\nAB' >exp2
pc_test
m4 in >out 2>err </dev/null || pc_fail "m4: nonzero status"
cmp out exp >/dev/null 2>&1 || pc_fail "diversions: output differs: $(cat out)"
m4 in2 >out2 2>err </dev/null || pc_fail "m4wrap: nonzero status"
cmp out2 exp2 >/dev/null 2>&1 || pc_fail "m4wrap: output differs: $(cat out2)"
pc_pass

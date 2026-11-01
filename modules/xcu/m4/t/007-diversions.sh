# divert, undivert, divnum, m4wrap
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.m4.007
# Diverted text is held and written at the end in buffer order; divert(-1)
# discards; divnum gives the current stream; undivert writes a buffer at
# once and empties it; m4wrap text is processed at end of input in call
# order.
# MUTANT    m4.divert_noarg_ignored
# MUTANT    m4.divert_neg_only1
# MUTANT    m4.divert_neg_stdout
# MUTANT    m4.divert_ignored
# MUTANT    m4.divnum_zero
# MUTANT    m4.divnum_neg_zero
# MUTANT    m4.divnum_plus1
# MUTANT    m4.undivert_ignored
# MUTANT    m4.undivert_noargs_none
# MUTANT    m4.undivert_all_reverse
# MUTANT    m4.undivert_first_only
# MUTANT    m4.undivert_keeps
# MUTANT    m4.undivert_rescan
# MUTANT    m4.undivert_to_stdout
# MUTANT    m4.undivert_adds_newline
# MUTANT    m4.m4wrap_ignored
# MUTANT    m4.m4wrap_unprocessed
# MUTANT    m4.m4wrap_returns_text
# MUTANT    m4.no_flush_divs
# MUTANT    m4.divs_reverse
# VARIANT   m4.engine
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
m4wrap(`A')dnl
body
EOM
printf 'body\nA' >exp2
cat >in3 <<'EOM'
divnum|
divert(-1)define(`dn',divnum)divert(0)dn
divert(2)divnum
divert
EOM
printf '0|\n-1\n\n2\n' >exp3
cat >in4 <<'EOM'
define(`m',M)divert(-2)gone divert(0)dnl
divert(1)one
divert(3)three
divert(2)two
divert(0)dnl
divert(5)undivert(3,1)divert(0)dnl
mid
undivert
EOM
printf 'mid\ntwo\nthree\none\n\n' >exp4
cat >in5 <<'EOM'
define(`m',M)divert(1)`m'
divert(0)dnl
undivert(1)dnl
EOM
printf 'm\n' >exp5
cat >in6 <<'EOM'
m4wrap(`ifelse(a,a,Y)')dnl
body
EOM
printf 'body\nY' >exp6
cat >in7 <<'EOM'
m4wrap(`define(`zq',9)zq')dnl
body
EOM
printf 'body\n9' >exp7
pc_test
m4 in >out 2>err </dev/null || pc_fail "m4: nonzero status"
cmp out exp >/dev/null 2>&1 || pc_fail "diversions: output differs: $(cat out)"
m4 in2 >out2 2>err </dev/null || pc_fail "m4wrap: nonzero status"
cmp out2 exp2 >/dev/null 2>&1 || pc_fail "m4wrap: output differs: $(cat out2)"
m4 in3 >out3 2>err </dev/null || pc_fail "divnum: nonzero status"
cmp out3 exp3 >/dev/null 2>&1 || pc_fail "divnum and divert without argument: output differs: $(cat out3)"
m4 in4 >out4 2>err </dev/null || pc_fail "undivert: nonzero status"
cmp out4 exp4 >/dev/null 2>&1 || pc_fail "negative streams, undivert into a stream, undivert without arguments: output differs: $(cat out4)"
m4 in5 >out5 2>err </dev/null || pc_fail "undivert of quoted text: nonzero status"
cmp out5 exp5 >/dev/null 2>&1 || pc_fail "undivert output was scanned again: $(cat out5)"
m4 in6 >out6 2>err </dev/null || pc_fail "m4wrap text: nonzero status"
cmp out6 exp6 >/dev/null 2>&1 || pc_fail "m4wrap text was not processed: $(cat out6)"
m4 in7 >out7 2>err </dev/null || pc_fail "m4wrap text with a definition: nonzero status"
cmp out7 exp7 >/dev/null 2>&1 || pc_fail "m4wrap text with a definition was not processed: $(cat out7)"
pc_pass

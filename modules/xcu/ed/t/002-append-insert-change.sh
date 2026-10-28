# a, i and c commands, address 0
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ed.002
# "2a", "0a", "$i" and "3c" with text lines end by a lone dot put the text
# after line 2, at the top, before the last line, and in place of line 3;
# the result written to a file has the expected eight lines.
. "$PC_CORE/pc.sh"
pc_begin XCU.ed.002
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
printf 'one\ntwo\nthree\nfour\nfive\n' >f || pc_unresolved "setup: cannot write f"
cat >s <<'EOS'
2a
ins-after-2
.
0a
first
.
$i
before-last
.
3c
changed
.
w g
q
EOS
cat >s2 <<'EOS'
2a
X
.
.=
$=
1c
A
.
.=
$c
Z
.
.=
Q
EOS
printf 'first\none\nchanged\nins-after-2\nthree\nfour\nbefore-last\nfive\n' >exp
printf '3\n6\n1\n6\n' >exp2
pc_test
ed -s f <s >o 2>err
cmp -s g exp || pc_fail "the edited file differs: $(cat g)"
ed -s f <s2 >o2 2>err
cmp -s o2 exp2 || pc_fail "current line numbers after a and c are '$(cat o2)', expected 3 6 1 6"
pc_pass

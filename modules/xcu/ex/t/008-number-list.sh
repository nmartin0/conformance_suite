# print flags, number, list and =
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ex.008
# nu and # precede lines by a six-wide number and two spaces, l shows tab as
# \t, escapes a dollar and ends with a dollar, = prints the line number, and
# set number makes p number lines.
. "$PC_CORE/pc.sh"
pc_begin XCU.ex.008
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
printf 'one\ntwo\nthree\nfour\nfive\n' >f || pc_unresolved "setup: cannot write f"
printf 'a\tb$c\n' >g || pc_unresolved "setup: cannot write g"
cat >s.ex <<'EOS'
2nu
2#
1,2nu
$=
3p #
set nu
4p
q
EOS
printf '%b' '     2  two\n     2  two\n     1  one\n     2  two\n5\n     3  three\n     4  four\n' >exp
cat >s2.ex <<'EOS'
l
q
EOS
printf '%b' 'a\\tb\\$c$\n' >exp2
pc_test
ex -s f <s.ex >out 2>err
rc=$?
[ "$rc" -eq 0 ] || pc_fail "ex: exit status $rc"
cmp out exp >/dev/null 2>&1 || pc_fail "the numbered lines differs: $(tr '\n' '|' <out)"
ex -s g <s2.ex >out 2>err
rc=$?
[ "$rc" -eq 0 ] || pc_fail "ex: exit status $rc"
cmp out exp2 >/dev/null 2>&1 || pc_fail "the list output differs: $(tr '\n' '|' <out)"
pc_pass

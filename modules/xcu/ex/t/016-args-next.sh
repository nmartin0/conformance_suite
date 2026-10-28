# next and args
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ex.016
# n moves to the next file operand after a write and ar shows the list with
# the current entry in brackets.
. "$PC_CORE/pc.sh"
pc_begin XCU.ex.016
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
printf 'a1\na2\n' >f1 || pc_unresolved "setup: cannot write f1"
printf 'b1\nb2\n' >f2 || pc_unresolved "setup: cannot write f2"
cat >s.ex <<'EOS'
ar
1d
w
n
1d
w
q
EOS
pc_test
ex -s f1 f2 <s.ex >out 2>err || pc_fail "ex f1 f2: nonzero status"
[ "$(cat f1)" = a2 ] || pc_fail "f1 is '$(cat f1)'"
[ "$(cat f2)" = b2 ] || pc_fail "f2 is '$(cat f2)' (n did not move to the second file)"
grep -F '[f1]' out >/dev/null || pc_fail "ar did not show [f1]: $(cat out)"
grep 'f2' out >/dev/null || pc_fail "ar did not list f2: $(cat out)"
pc_pass

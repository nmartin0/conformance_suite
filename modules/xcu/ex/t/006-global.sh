# global and v
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ex.006
# g runs commands on matching lines, v on the others, g/^/m0 reverses the
# buffer, and g! is v.
. "$PC_CORE/pc.sh"
pc_begin XCU.ex.006
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
printf 'a1\nb1\na2\nb2\na3\n' >f || pc_unresolved "setup: cannot write f"
cat >s.ex <<'EOS'
g/a/s/$/!/
v/!/d
g/a2/m0
g/^/m0
%p
g!/a1/d
w
q
EOS
printf '%b' 'a3!\na1!\na2!\n' >expout
printf '%b' 'a1!\n' >exp
pc_test
ex -s f <s.ex >out 2>err
rc=$?
[ "$rc" -eq 0 ] || pc_fail "ex: exit status $rc"
cmp out expout >/dev/null 2>&1 || pc_fail "the printed lines differs: $(tr '\n' '|' <out)"
cmp f exp >/dev/null 2>&1 || pc_fail "the edited file differs: $(tr '\n' '|' <f)"
pc_pass

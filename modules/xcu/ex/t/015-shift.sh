# shift left and right
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ex.015
# > indents by shiftwidth columns and < removes it.
. "$PC_CORE/pc.sh"
pc_begin XCU.ex.015
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
printf 'one\ntwo\nthree\nfour\nfive\n' >f || pc_unresolved "setup: cannot write f"
cat >s.ex <<'EOS'
set sw=4
2>
w
q
EOS
cat >s2.ex <<'EOS'
set sw=4
2>
2<
w
q
EOS
pc_test
ex -s f <s.ex >out 2>err || pc_fail "ex: nonzero status"
sed -n 2p f >l2
[ "$(cat l2)" = '    two' ] || pc_fail "> with shiftwidth 4 gave '$(cat l2)'"
printf 'one\ntwo\nthree\nfour\nfive\n' >f
ex -s f <s2.ex >out 2>err || pc_fail "ex (shift back): nonzero status"
sed -n 2p f >l2
[ "$(cat l2)" = two ] || pc_fail "< did not remove the indentation: '$(cat l2)'"
pc_pass

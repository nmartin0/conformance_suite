# ar -s is accepted
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ar.016
# "ar -s a" on an existing archive exits 0 and the members are unchanged and
# still listed in order.
. "$PC_CORE/pc.sh"
pc_begin XCU.ar.016
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
printf 'one\n' >f1; printf 'two\n' >f2
ar -rc a f1 f2 </dev/null >/dev/null 2>&1 || pc_unresolved "setup: cannot build the archive"
pc_test
ar -s a >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "ar -s: exit status $rc"
[ "$(ar -t a </dev/null | tr '\n' ' ')" = 'f1 f2 ' ] || pc_fail "the members changed after ar -s"
[ "$(ar -p a f2 </dev/null)" = two ] || pc_fail "member contents changed after ar -s"
pc_pass

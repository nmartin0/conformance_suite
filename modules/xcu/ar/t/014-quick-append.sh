# ar -q appends
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ar.014
# "ar -q a f2" appends f2 after the existing members.
. "$PC_CORE/pc.sh"
pc_begin XCU.ar.014
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
printf 'one\n' >f1; printf 'two\n' >f2
ar -rc a f1 </dev/null >/dev/null 2>&1 || pc_unresolved "setup: cannot build the archive"
pc_test
ar -q a f2 >out 2>err </dev/null || pc_fail "ar -q: nonzero status"
[ "$(ar -t a </dev/null | tr '\n' ' ')" = 'f1 f2 ' ] || pc_fail "ar -q did not append f2"
[ "$(ar -p a f2 </dev/null)" = two ] || pc_fail "the appended member has the wrong contents"
pc_pass

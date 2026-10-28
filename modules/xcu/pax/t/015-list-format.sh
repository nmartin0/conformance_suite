# pax -o listopt
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.pax.015
# "-o listopt=%(size)d:%F" in list mode writes, for each member, its size in
# decimal, a colon and its path, then a newline.
. "$PC_CORE/pc.sh"
pc_begin XCU.pax.015
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
printf 'abcde\n' >f1
pax -w f1 >a 2>/dev/null </dev/null || pc_unresolved "setup: pax -w failed"
pc_test
pax -v -o 'listopt=%(size)d:%F' <a >o 2>err || pc_fail "pax -o listopt: nonzero status"
[ "$(cat o)" = '6:f1' ] || pc_fail "listopt output is '$(cat o)', expected 6:f1"
pc_pass

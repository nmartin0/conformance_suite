# pax -d
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.pax.004
# "pax -w -d d" archives only the directory d itself; in list mode "-d d" on
# a full archive lists only d, while without -d the pattern d also lists
# d/f2.
. "$PC_CORE/pc.sh"
pc_begin XCU.pax.004
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
mkdir d; printf 'nested\n' >d/f2
pax -w d >full 2>/dev/null </dev/null || pc_unresolved "setup: pax -w failed"
pc_test
pax -w -d d >dd 2>err </dev/null || pc_fail "pax -w -d: nonzero status"
pax <dd >o 2>err || pc_fail "list of the -d archive: nonzero status"
[ "$(sed 's,/*$,,' o | tr '\n' ' ')" = 'd ' ] || pc_fail "pax -w -d archived '$(tr '\n' ' ' <o)'"
pax -d d <full >o 2>err || pc_fail "pax -d d (list): nonzero status"
[ "$(sed 's,/*$,,' o | tr '\n' ' ')" = 'd ' ] || pc_fail "pax -d d listed '$(tr '\n' ' ' <o)'"
pax d <full >o 2>err || pc_fail "pax d (list): nonzero status"
[ "$(sed 's,/*$,,' o | sort | tr '\n' ' ')" = 'd d/f2 ' ] || pc_fail "pax d listed '$(tr '\n' ' ' <o)'"
pc_pass

# write mode reads file names from standard input
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.pax.005
# "pax -w" with no file operands reads one pathname per line from standard
# input and archives those files.
. "$PC_CORE/pc.sh"
pc_begin XCU.pax.005
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
printf 'one\n' >f1; printf 'three\n' >f3
pc_test
printf 'f1\nf3\n' | pax -w >a 2>err || pc_fail "pax -w with names on standard input: nonzero status"
pax <a >o 2>err || pc_fail "list: nonzero status"
[ "$(sort o | tr '\n' ' ')" = 'f1 f3 ' ] || pc_fail "the archive lists '$(tr '\n' ' ' <o)'"
pc_pass

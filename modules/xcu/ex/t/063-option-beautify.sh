# the beautify option
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ex.063
. "$PC_CORE/pc.sh"
pc_begin XCU.ex.063
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
printf 'a\001b\tc\n' >ctl
printf 'x\n' >f
pc_test
printf 'set beautify\nr ctl\nw\nq\n' >s
ex -s f <s >o 2>e || pc_fail "ex: exit status $?"
printf 'x\nab\tc\n' >exp
cmp -s f exp || pc_fail "beautify did not drop the control character: $(tr '\n\t\001' '|T^' <f)"
printf 'x\n' >f
printf 'r ctl\nw\nq\n' >s2
ex -s f <s2 >o 2>e || pc_fail "ex: exit status $?"
printf 'x\na\001b\tc\n' >exp
cmp -s f exp || pc_fail "without beautify the control character was changed: $(tr '\n\t\001' '|T^' <f)"
pc_pass

# omitted closing delimiters
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ed.021
# A closing delimiter that would end the line may be left out: s then
# prints the result, g and v print the lines, a ?RE address is a search, and an empty
# line is +1p.
. "$PC_CORE/pc.sh"
pc_begin XCU.ed.021
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
printf 'one\ntwo\nthree\nfour\nfive\n' >f || pc_unresolved "setup: cannot write f"
printf '1s/one/uno\ng/^t\nv/f\n?o\n\n' >s
printf 'uno\ntwo\nthree\nuno\ntwo\nthree\ntwo\nthree\n' >exp
pc_test
ed -s f <s >o 2>err
cmp -s o exp || pc_fail "omitted delimiters gave '$(tr '\n' ' ' <o)', expected '$(tr '\n' ' ' <exp)'"
pc_pass

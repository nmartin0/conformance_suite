# an address alone and an empty line
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ed.005
# An address alone prints that line and makes it current; an empty command
# line acts as +1p.
. "$PC_CORE/pc.sh"
pc_begin XCU.ed.005
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
printf 'one\ntwo\nthree\nfour\nfive\n' >f || pc_unresolved "setup: cannot write f"
printf '2\n\n\nQ\n' >s
printf 'two\nthree\nfour\n' >exp
pc_test
ed -s f <s >o 2>err
cmp -s o exp || pc_fail "address-only and empty lines printed: $(cat o)"
pc_pass

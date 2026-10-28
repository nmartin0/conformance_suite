# option syntax
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ed.027
# -s and -p STR (attached or separate) may precede the file, and -- ends the
# options.
. "$PC_CORE/pc.sh"
pc_begin XCU.ed.027
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
printf 'one\ntwo\nthree\nfour\nfive\n' >f || pc_unresolved "setup: cannot write f"
printf '1p\nq\n' >s
printf '> one\n> ' >exp2
pc_test
ed -s -- f <s >o1 2>err
[ "$(cat o1)" = one ] || pc_fail "ed -s -- f gave '$(cat o1)'"
ed -s -p '> ' f <s >o2 2>err
cmp -s o2 exp2 || pc_fail "-p '> ' gave '$(cat o2)'"
ed -s -p'> ' -- f <s >o3 2>err
cmp -s o2 o3 || pc_fail "-p attached and separate differ: '$(cat o3)' and '$(cat o2)'"
pc_pass

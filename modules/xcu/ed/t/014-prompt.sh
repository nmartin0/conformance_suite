# -p and P
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ed.014
# With -p STR the string is written before each command is read; P turns the
# prompt on with "*" when -p was not given.
. "$PC_CORE/pc.sh"
pc_begin XCU.ed.014
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
printf 'one\ntwo\nthree\nfour\nfive\n' >f || pc_unresolved "setup: cannot write f"
printf '1p\nq\n' >s
printf 'P\n1p\nq\n' >s2
printf 'P> one\nP> ' >exp
printf '*one\n*' >exp2
pc_test
ed -s -p 'P> ' f <s >o 2>err
cmp -s o exp || pc_fail "-p output was '$(cat o)'"
ed -s f <s2 >o2 2>err
cmp -s o2 exp2 || pc_fail "P output was '$(cat o2)'"
pc_pass

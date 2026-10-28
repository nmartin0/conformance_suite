# Utilities process environment variable names and values containing bytes with the high bit set without altering them
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.1.4-utility-defaults.004
. "$PC_CORE/pc.sh"
pc_begin XCU.1.4-utility-defaults.004
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
v=$(printf 'v\200\377w') || pc_unresolved "cannot build the value"
n=$(printf 'N\200\377') || pc_unresolved "cannot build the name"
printf 'PCA=%s\n' "$v" >w1 || pc_unresolved "write"
printf '%s=1\n' "$n" >w2 || pc_unresolved "write"
pc_test
env "PCA=$v" env >e1 2>err || pc_fail "env with a high-bit value failed"
grep -F -x -f w1 e1 >/dev/null 2>&1 || pc_fail "the value was altered"
env "$n=1" env >e2 2>err || pc_fail "env with a high-bit name failed"
grep -F -x -f w2 e2 >/dev/null 2>&1 || pc_fail "the name was altered"
pc_pass

# expr evaluates an expression nested in 32 levels of parentheses
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.1.2-utility-limits.004
. "$PC_CORE/pc.sh"
pc_begin XCU.1.2-utility-limits.004
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
pc_test
set --
i=0
while [ "$i" -lt 32 ]; do set -- "$@" '('; i=$((i + 1)); done
set -- "$@" 7
i=0
while [ "$i" -lt 32 ]; do set -- "$@" ')'; i=$((i + 1)); done
r=$(expr "$@" 2>/dev/null) || pc_fail "expr failed on 32 nested parentheses"
[ "$r" = 7 ] || pc_fail "expr gave '$r', not 7"
pc_pass

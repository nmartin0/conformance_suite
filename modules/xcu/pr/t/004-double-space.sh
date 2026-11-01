# -d adds an extra newline after every newline of the input.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.pr.004
# MUTANT    pr.d_ignored
# MUTANT    pr.d_nolast
# MUTANT    pr.d_triple
# MUTANT    pr.d_odd_only
# MUTANT    pr.d_blank_not_doubled
. "$PC_CORE/pc.sh"
pc_begin XCU.pr.004
LC_ALL=POSIX
export LC_ALL
NL='
'
printf 'a\nb\n' >f || pc_unresolved "cannot create input"
printf 'a\n\nb\n' >g || pc_unresolved "cannot create input"
pc_test
got=$(pr -t -d f 2>/dev/null </dev/null; printf x)
[ "$got" = "a${NL}${NL}b${NL}${NL}x" ] || pc_fail "-d did not add a newline after every input line"
got=$(pr -t -d g 2>/dev/null </dev/null; printf x)
[ "$got" = "a${NL}${NL}${NL}${NL}b${NL}${NL}x" ] || pc_fail "-d did not add a newline after the newline of an empty input line"
pc_pass

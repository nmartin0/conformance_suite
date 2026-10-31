# uniq -u writes only the lines that are not repeated.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.uniq.004
# MUTANT    uniq.u_all
# MUTANT    uniq.u_inverted
# MUTANT    uniq.u_ge3
# MUTANT    uniq.u_drops_last
# MUTANT    uniq.u_drops_first
# MUTANT    uniq.nonadj_u
# MUTANT    uniq.no_newline
# MUTANT    uniq.status1
. "$PC_CORE/pc.sh"
pc_begin XCU.uniq.004
LC_ALL=POSIX
export LC_ALL
NL='
'
printf 'a\na\nb\nc\nc\nc\nd\ne\nf\ne\n' >in || pc_fail "cannot create input"
pc_test
got=$(uniq -u in 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "b${NL}d${NL}e${NL}f${NL}e${NL}rc=0" ] || pc_fail "-u did not write exactly the unrepeated lines"
pc_pass

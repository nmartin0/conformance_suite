# uniq -d writes one copy of each line that is repeated and none of the others.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.uniq.003
# MUTANT    uniq.d_all
# MUTANT    uniq.d_inverted
# MUTANT    uniq.d_ge3
# MUTANT    uniq.d_exactly2
# MUTANT    uniq.d_every_copy
# MUTANT    uniq.d_first_only
# MUTANT    uniq.nonadj_d
# MUTANT    uniq.no_newline
# MUTANT    uniq.status1
. "$PC_CORE/pc.sh"
pc_begin XCU.uniq.003
LC_ALL=POSIX
export LC_ALL
NL='
'
printf 'a\na\nb\nc\nc\nc\nd\ne\nf\ne\n' >in || pc_fail "cannot create input"
pc_test
got=$(uniq -d in 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "a${NL}c${NL}rc=0" ] || pc_fail "-d did not write exactly one copy of each repeated line"
pc_pass

# A line that is not numbered is written as the input line alone, with no number and no separator.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.nl.002
. "$PC_CORE/pc.sh"
pc_begin XCU.nl.002
LC_ALL=POSIX
export LC_ALL
NL='
'
T=$(printf '\t')
printf 'a\n\nb\n' >in || pc_fail "cannot create input"
pc_test
got=$(nl in 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "     1${T}a${NL}${NL}     2${T}b${NL}rc=0" ] || pc_fail "the empty line was not written as an empty line"
got=$(nl -b n in 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "a${NL}${NL}b${NL}rc=0" ] || pc_fail "-b n did not leave the lines unchanged"
pc_pass

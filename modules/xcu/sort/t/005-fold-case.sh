# sort -f compares lowercase letters as their uppercase equivalents; lines that then tie are ordered by byte value.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.sort.005
. "$PC_CORE/pc.sh"
pc_begin XCU.sort.005
LC_ALL=POSIX
export LC_ALL
NL='
'
printf 'b\nA\na\nB\nC\n' >in || pc_fail "cannot create input"
pc_test
got=$(sort -f in 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "A${NL}a${NL}B${NL}b${NL}C${NL}rc=0" ] || pc_fail "-f order is wrong"
pc_pass

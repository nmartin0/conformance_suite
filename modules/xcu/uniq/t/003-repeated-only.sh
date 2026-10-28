# uniq -d writes one copy of each line that is repeated and none of the others.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.uniq.003
. "$PC_CORE/pc.sh"
pc_begin XCU.uniq.003
LC_ALL=POSIX
export LC_ALL
NL='
'
printf 'a\na\nb\nc\nc\nc\nd\n' >in || pc_fail "cannot create input"
pc_test
got=$(uniq -d in 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "a${NL}c${NL}rc=0" ] || pc_fail "-d did not write exactly one copy of each repeated line"
pc_pass

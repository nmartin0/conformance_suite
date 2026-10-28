# files and pipes with identical names
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.awk.031
# Output files and pipes with textually identical names are one stream, and
# names that differ even by a blank are different ones.
. "$PC_CORE/pc.sh"
pc_begin XCU.awk.031
LC_ALL=POSIX
export LC_ALL
NL='
'
pc_test
awk 'BEGIN { print "a" > "pcf"; print "b" > "pcf"; close("pcf"); print "c" > "pcf2 "; print "d" > "pcf2"; close("pcf2 "); close("pcf2") }' 2>/dev/null </dev/null || pc_fail "awk: nonzero status"
[ "$(cat pcf 2>/dev/null)" = "a${NL}b" ] || pc_fail "two prints to the same name did not share the file: $(cat pcf 2>/dev/null)"
[ "$(cat 'pcf2 ' 2>/dev/null)" = "c" ] || pc_fail "the name with a trailing blank was not a file of its own"
[ "$(cat pcf2 2>/dev/null)" = "d" ] || pc_fail "the name without the blank was not a file of its own"
got=$(awk 'BEGIN { print "b" | "sort"; print "a" | "sort"; close("sort"); print "end" }' 2>/dev/null </dev/null)
[ "$got" = "a${NL}b${NL}end" ] || pc_fail "two prints to the same command did not share the pipe: $got"
pc_pass

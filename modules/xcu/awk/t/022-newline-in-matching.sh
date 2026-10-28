# matching text that holds newlines
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.awk.022
# With a record separator other than newline a record can hold a newline
# that an ERE matches; with the default separator no expression matches the
# separator; against an ordinary string any character, newline included, can
# be matched and a dot matches it.
. "$PC_CORE/pc.sh"
pc_begin XCU.awk.022
LC_ALL=POSIX
export LC_ALL
NL='
'
pc_test
got=$(printf 'a\nb;c' | awk 'BEGIN { RS = ";" } /a\nb/ { print "m" }' 2>/dev/null)
[ "$got" = "m" ] || pc_fail "a newline inside a record was not matched: $got"
got=$(printf 'x\ny\n' | awk '/x\ny/ { print "bad" } /x.y/ { print "bad2" } END { print NR }' 2>/dev/null)
[ "$got" = "2" ] || pc_fail "an ERE matched across the record separator: $got"
got=$(awk 'BEGIN { s = "a\nb"; m = match(s, /[^x]+/); print (s ~ /a.b/), (s ~ /a\nb/), (s ~ "a\nb"), m, RLENGTH }' 2>/dev/null </dev/null)
[ "$got" = "1 1 1 1 3" ] || pc_fail "matching a string with a newline: $got"
got=$(awk 'BEGIN { RS = ";"; s = "a;b"; print (s ~ /a;b/) }' 2>/dev/null </dev/null)
[ "$got" = "1" ] || pc_fail "an ordinary string could not hold the record separator in a match: $got"
pc_pass

# awk regular expression constants and dynamic regular expressions use extended syntax with the specified escapes, /re/ alone tests the record, and range patterns are inclusive and repeat.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.awk.016
. "$PC_CORE/pc.sh"
pc_begin XCU.awk.016
LC_ALL=POSIX
export LC_ALL
NL='
'
pc_test
e() { awk "BEGIN { $1 }" 2>/dev/null </dev/null; }
[ "$(echo 'a.c abc' | awk '{ print ($1 ~ /a\.c/), ($2 ~ /a\.c/) }' 2>/dev/null)" = "1 0" ] || pc_fail "an escaped dot in an ERE constant wrong"
[ "$(e 'r = "^a.c$"; print ("abc" ~ r), ("abcd" ~ r)')" = "1 0" ] || pc_fail "a dynamic regular expression wrong"
[ "$(e 'print ("a.c" ~ "a\\.c"), ("abc" ~ "a\\.c")')" = "1 0" ] || pc_fail "escapes in a string used as a regular expression wrong"
[ "$(echo foo | awk '{ x = /o+/; y = /z/; print x, y }' 2>/dev/null)" = "1 0" ] || pc_fail "/re/ as an expression did not test \$0"
[ "$(echo a/b | awk '/a\/b/ { print "ok" }' 2>/dev/null)" = "ok" ] || pc_fail "an escaped slash in an ERE constant wrong"
[ "$(printf 'a\tb\n' | awk '/a[\t]b/ { print "tab" }' 2>/dev/null)" = "tab" ] || pc_fail "an escape inside a bracket expression wrong"
[ "$(e 'printf "\101\n"')" = "A" ] || pc_fail "an octal escape in a string wrong"
[ "$(e 'print ("aab" ~ /^a+b$/), ("ab" ~ /^(a|b)+$/), ("abab" ~ /^(ab){2}$/), ("x" ~ /[[:alpha:]]/)')" = "1 1 1 1" ] || pc_fail "ERE operators wrong"
[ "$(printf 'a\nb\n' | awk '!/b/' 2>/dev/null)" = "a" ] || pc_fail "negated pattern wrong"
[ "$(printf 'a\nb\n' | awk '$0 ~ "^a"' 2>/dev/null)" = "a" ] || pc_fail "\$0 ~ string pattern wrong"
[ "$(printf '1\n2\n3\n4\n5\n' | awk '/2/,/3/' 2>/dev/null)" = "2${NL}3" ] || pc_fail "an RE range wrong"
[ "$(printf '1\n2\n3\n4\n5\n' | awk 'NR==2,NR==3 { print "r" $0 }' 2>/dev/null)" = "r2${NL}r3" ] || pc_fail "a numeric range wrong"
[ "$(printf '1\n2\n3\n' | awk '$1==2,$1==2' 2>/dev/null)" = "2" ] || pc_fail "a range starting and ending on one record wrong"
[ "$(printf 'a\nb\na\nb\nc\n' | awk '/a/,/b/' 2>/dev/null)" = "a${NL}b${NL}a${NL}b" ] || pc_fail "a range was not repeated"
[ "$(printf 'a\nc\nd\n' | awk '/c/,/zzz/' 2>/dev/null)" = "c${NL}d" ] || pc_fail "a range never ended did not run to the end"
[ "$(printf 'a\nb\na\n' | awk '/a/,/a/' 2>/dev/null)" = "a${NL}a" ] || pc_fail "a range whose ends match the same record wrong"
pc_pass

# escape sequences in strings and regular expressions
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.awk.021
# A string constant takes the escapes \a \b \f \n \r \t \v \" \\ and
# octal \ddd with the longest one to three digits; in a regular expression the
# escapes work also inside brackets, \/ is a slash and a backslash before a
# special character makes it literal, in constants and in dynamic strings.
. "$PC_CORE/pc.sh"
pc_begin XCU.awk.021
LC_ALL=POSIX
export LC_ALL
NL='
'
pc_test
got=$(awk 'BEGIN { printf "%s", "\a\b\f\n\r\t\v\"\\\101\60\1011" }' 2>/dev/null </dev/null | od -An -tx1 | tr -d ' \n')
[ "$got" = "07080c0a0d090b225c41304131" ] || pc_fail "string escapes gave $got"
got=$(echo 'a.b axb' | awk '{ print ($1 ~ /a\.b/), ($2 ~ /a\.b/), ($1 ~ "a\\.b"), ($2 ~ "a\\.b") }' 2>/dev/null)
[ "$got" = "1 0 1 0" ] || pc_fail "a backslash before a dot in an ERE: $got"
got=$(echo 'a/b' | awk '{ print ($0 ~ /a\/b/), ($0 ~ "a/b") }' 2>/dev/null)
[ "$got" = "1 1" ] || pc_fail "\\/ in an ERE constant: $got"
got=$(printf 'a\\b\n' | awk '{ print ($0 ~ /a\\b/), ($0 ~ "a\\\\b") }' 2>/dev/null)
[ "$got" = "1 1" ] || pc_fail "an escaped backslash in an ERE: $got"
got=$(printf 'a\tb\n' | awk '/[\t]/ { print "tab" } /a\tb/ { print "tab2" }' 2>/dev/null)
[ "$got" = "tab${NL}tab2" ] || pc_fail "\\t in an ERE or bracket expression: $got"
got=$(echo 'a+b a(b a*b' | awk '{ print ($1 ~ /a\+b/), ($2 ~ /a\(b/), ($3 ~ /a\*b/), ("aab" ~ /a\+b/) }' 2>/dev/null)
[ "$got" = "1 1 1 0" ] || pc_fail "escaped special characters in an ERE: $got"
got=$(echo 'A' | awk '{ print ($0 ~ /\101/), ($0 ~ "\101") }' 2>/dev/null)
[ "$got" = "1 1" ] || pc_fail "an octal escape in an ERE: $got"
pc_pass

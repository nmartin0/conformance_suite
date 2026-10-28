# records and output of LINE_MAX bytes
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.awk.029
# Records of up to {LINE_MAX} bytes are supported whether the record
# separator is a newline or another character, and print and printf can
# write {LINE_MAX} bytes.
. "$PC_CORE/pc.sh"
pc_begin XCU.awk.029
LC_ALL=POSIX
export LC_ALL
NL='
'
n=$(getconf LINE_MAX 2>/dev/null)
case $n in ''|*[!0-9]*) n=2048 ;; esac
mkx() { s=x; while [ "${#s}" -lt "$1" ]; do s=$s$s; done; printf "%.${1}s" "$s"; }
mkx $((n - 1)) >rec.body || pc_fail "cannot make the test record"
{ cat rec.body; printf '\n'; } >rec.nl
{ cat rec.body; printf ';'; } >rec.semi
pc_test
got=$(awk '{ print length($0) }' rec.nl 2>/dev/null </dev/null)
[ "$got" = "$((n - 1))" ] || pc_fail "a newline-terminated record of $n bytes: length $got"
got=$(awk 'BEGIN { RS = ";" } { print length($0) }' rec.semi 2>/dev/null </dev/null)
[ "$got" = "$((n - 1))" ] || pc_fail "a record of $n bytes ended by another separator: length $got"
got=$(awk -v n="$n" 'BEGIN { s = "x"; while (length(s) < n) s = s s; s = substr(s, 1, n); print s }' 2>/dev/null </dev/null | wc -c | tr -d ' ')
[ "$got" = "$((n + 1))" ] || pc_fail "print of $n bytes wrote $got bytes"
got=$(awk -v n="$n" 'BEGIN { s = "x"; while (length(s) < n) s = s s; s = substr(s, 1, n); printf "%s", s }' 2>/dev/null </dev/null | wc -c | tr -d ' ')
[ "$got" = "$n" ] || pc_fail "printf of $n bytes wrote $got bytes"
pc_pass

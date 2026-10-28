# wc -l, -w and -c each write just that count and the file name.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.wc.002
. "$PC_CORE/pc.sh"
pc_begin XCU.wc.002
LC_ALL=POSIX
export LC_ALL
NL='
'
printf 'one two\nthree\n\nfour five six\n' >f || pc_fail "cannot create input"
pc_test
out=$(wc -l f 2>/dev/null </dev/null)
rc=$?
set -- $out
[ "$rc" -eq 0 ] && [ $# -eq 2 ] && [ "$1" = 4 ] && [ "$2" = f ] || pc_fail "wc -l f is not '4 f'"
out=$(wc -w f 2>/dev/null </dev/null)
rc=$?
set -- $out
[ "$rc" -eq 0 ] && [ $# -eq 2 ] && [ "$1" = 6 ] && [ "$2" = f ] || pc_fail "wc -w f is not '6 f'"
out=$(wc -c f 2>/dev/null </dev/null)
rc=$?
set -- $out
[ "$rc" -eq 0 ] && [ $# -eq 2 ] && [ "$1" = 29 ] && [ "$2" = f ] || pc_fail "wc -c f is not '29 f'"
pc_pass

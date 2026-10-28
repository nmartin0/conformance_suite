# ls -lR: the directory header comes before the total line
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ls.052
. "$PC_CORE/pc.sh"
pc_begin XCU.ls.052
LC_ALL=POSIX; export LC_ALL
mkdir d d/sub || pc_unresolved "cannot create d/sub"
: >d/a && : >d/sub/b || pc_unresolved "cannot create the files"
pc_test
ls -lR d >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "ls -lR d exited with status $rc"
n=$(awk '$0 == "d/sub:" { print NR; exit }' out)
[ -n "$n" ] || pc_fail "no header line d/sub: in the output: $(cat out)"
[ "$n" -ge 2 ] || pc_fail "the header d/sub: is the first line, expected a blank line before it"
prev=$(sed -n "$((n - 1))p" out)
[ -z "$prev" ] || pc_fail "the line before the header is '$prev', expected a blank line"
next=$(sed -n "$((n + 1))p" out)
case $next in
"total "[0-9]*) ;;
*) pc_fail "the line after the header is '$next', expected the total line" ;;
esac
pc_pass

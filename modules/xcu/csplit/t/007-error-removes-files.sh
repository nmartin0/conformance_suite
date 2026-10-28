# When csplit fails it exits nonzero and, without -k, leaves none of the files it made.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.csplit.007
. "$PC_CORE/pc.sh"
pc_begin XCU.csplit.007
LC_ALL=POSIX
export LC_ALL
NL='
'
printf '%s\n' 1 2 3 4 5 6 7 8 9 10 >in10 || pc_fail "cannot create input"
pc_test
csplit -s in10 3 '/nomatchatall/' >/dev/null 2>&1 </dev/null
rc=$?
[ "$rc" -ne 0 ] || pc_fail "status 0 although the expression matched no line"
for f in xx00 xx01 xx02; do
	[ -e "$f" ] && pc_fail "$f was left after the error"
done
csplit -s in10 3 20 >/dev/null 2>&1 </dev/null
rc=$?
[ "$rc" -ne 0 ] || pc_fail "status 0 for a line number past the end of the file"
for f in xx00 xx01 xx02; do
	[ -e "$f" ] && pc_fail "$f was left after the line number error"
done
pc_pass

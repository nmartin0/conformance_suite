# A newline follows the formatted text; ordinary text and %n %t %% work.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.date.002
. "$PC_CORE/pc.sh"
pc_begin XCU.date.002
LC_ALL=C; export LC_ALL
TZ=UTC0; export TZ
nl='
'
tab=$(printf '\t')
# rd FILE: v = exact content of FILE (the x keeps trailing newlines).
rd() {
	v=$(cat "$1"; printf x)
	v=${v%x}
}
pc_test
date '+' >o1 2>/dev/null
rd o1
[ "$v" = "$nl" ] || pc_fail "date '+' did not write exactly one newline"
date '+a b%%c%nd%te' >o2 2>/dev/null
rd o2
[ "$v" = "a b%c${nl}d${tab}e${nl}" ] || pc_fail "ordinary text, %n, %t or %% handled wrongly"
date '+%n' >o3 2>/dev/null
rd o3
[ "$v" = "${nl}${nl}" ] || pc_fail "no newline was appended after a format ending in %n"
date '+plain' >o4 2>/dev/null
rd o4
[ "$v" = "plain${nl}" ] || pc_fail "'+plain' did not give plain and one newline"
pc_pass

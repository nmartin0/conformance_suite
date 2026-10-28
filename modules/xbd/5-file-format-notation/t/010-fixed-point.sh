# The f and F conversions.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XBD.5-file-format-notation.010
. "$PC_CORE/pc.sh"
pc_begin XBD.5-file-format-notation.010
LC_ALL=POSIX
export LC_ALL
pc_test
aw() { # <format> <argument> <expected>: the awk printf statement
	got=$(awk -v f="$1" -v a="$2" 'BEGIN { printf f, a }' 2>/dev/null </dev/null)
	[ "$got" = "$3" ] || pc_fail "awk printf \"$1\", \"$2\" wrote '$got', expected '$3'"
}
aw '%f|' 3.14159 '3.141590|'
aw '%.2f|' 3.14159 '3.14|'
aw '%.0f|' 3 '3|'
aw '%#.0f|' 3 '3.|'
aw '%+.1f|' 2 '+2.0|'
aw '% .1f|' 2 ' 2.0|'
aw '%8.3f|' 3.14159 '   3.142|'
aw '%-8.1f|' 2.5 '2.5     |'
aw '%08.2f|' -3.14159 '-0003.14|'
aw '%+08.2f|' 3.14159 '+0003.14|'
aw '%F|' 1.5 '1.500000|'
aw '%.3f|' -0.5 '-0.500|'
pc_pass

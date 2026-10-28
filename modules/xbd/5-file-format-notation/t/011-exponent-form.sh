# The e and E conversions.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XBD.5-file-format-notation.011
. "$PC_CORE/pc.sh"
pc_begin XBD.5-file-format-notation.011
LC_ALL=POSIX
export LC_ALL
pc_test
aw() { # <format> <argument> <expected>: the awk printf statement
	got=$(awk -v f="$1" -v a="$2" 'BEGIN { printf f, a }' 2>/dev/null </dev/null)
	[ "$got" = "$3" ] || pc_fail "awk printf \"$1\", \"$2\" wrote '$got', expected '$3'"
}
aw '%e|' 1234.5 '1.234500e+03|'
aw '%E|' 1234.5 '1.234500E+03|'
aw '%.2e|' 0.000123 '1.23e-04|'
aw '%.0e|' 5000 '5e+03|'
aw '%#.0e|' 5000 '5.e+03|'
aw '%.1e|' 1e100 '1.0e+100|'
aw '%e|' 0 '0.000000e+00|'
aw '%12.3e|' -1234.56 '  -1.235e+03|'
aw '%-12.3e|' 1234.56 '1.235e+03   |'
pc_pass

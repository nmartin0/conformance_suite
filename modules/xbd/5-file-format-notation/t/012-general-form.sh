# The g and G conversions.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XBD.5-file-format-notation.012
. "$PC_CORE/pc.sh"
pc_begin XBD.5-file-format-notation.012
LC_ALL=POSIX
export LC_ALL
pc_test
aw() { # <format> <argument> <expected>: the awk printf statement
	got=$(awk -v f="$1" -v a="$2" 'BEGIN { printf f, a }' 2>/dev/null </dev/null)
	[ "$got" = "$3" ] || pc_fail "awk printf \"$1\", \"$2\" wrote '$got', expected '$3'"
}
aw '%g|' 100000 '100000|'
aw '%g|' 1000000 '1e+06|'
aw '%g|' 0.0001 '0.0001|'
aw '%g|' 0.00001 '1e-05|'
aw '%G|' 0.00001 '1E-05|'
aw '%g|' 0.5 '0.5|'
aw '%g|' 100 '100|'
aw '%g|' 1.5 '1.5|'
aw '%g|' 123456789 '1.23457e+08|'
aw '%#g|' 0.5 '0.500000|'
aw '%.3g|' 1234.5 '1.23e+03|'
aw '%.3g|' 12.345 '12.3|'
pc_pass

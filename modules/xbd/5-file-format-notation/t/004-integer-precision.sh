# The precision of the integer conversions is the least number of digits.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XBD.5-file-format-notation.004
. "$PC_CORE/pc.sh"
pc_begin XBD.5-file-format-notation.004
LC_ALL=POSIX
export LC_ALL
pc_test
both() {
	got=$(printf "$1" "$2" 2>/dev/null </dev/null)
	[ "$got" = "$3" ] || pc_fail "printf '$1' '$2' wrote '$got', expected '$3'"
	got=$(awk -v f="$1" -v a="$2" 'BEGIN { printf f, a }' 2>/dev/null </dev/null)
	[ "$got" = "$3" ] || pc_fail "awk printf \"$1\", \"$2\" wrote '$got', expected '$3'"
}
both '%.5x|' 255 '000ff|'
both '%.3X|' 255 '0FF|'
both '%.4o|' 8 '0010|'
both '%.2u|' 5 '05|'
both '%.3d|' 7 '007|'
both '%.3d|' -7 '-007|'
both '%+.3d|' 7 '+007|'
both '%.3i|' 7 '007|'
both '%.1d|' 123 '123|'
both '%.1x|' 255 'ff|'
pc_pass

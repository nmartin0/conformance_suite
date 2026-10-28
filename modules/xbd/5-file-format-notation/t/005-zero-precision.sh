# Zero with precision 0 writes nothing; zero without a precision writes one digit.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XBD.5-file-format-notation.005
. "$PC_CORE/pc.sh"
pc_begin XBD.5-file-format-notation.005
LC_ALL=POSIX
export LC_ALL
pc_test
both() {
	got=$(printf "$1" "$2" 2>/dev/null </dev/null)
	[ "$got" = "$3" ] || pc_fail "printf '$1' '$2' wrote '$got', expected '$3'"
	got=$(awk -v f="$1" -v a="$2" 'BEGIN { printf f, a }' 2>/dev/null </dev/null)
	[ "$got" = "$3" ] || pc_fail "awk printf \"$1\", \"$2\" wrote '$got', expected '$3'"
}
both '%.0d|' 0 '|'
both '%.d|' 0 '|'
both '%.0x|' 0 '|'
both '%.0o|' 0 '|'
both '%.0u|' 0 '|'
both '%.0i|' 0 '|'
both '%.0d|' 5 '5|'
both '%.d|' 5 '5|'
both '%d|' 0 '0|'
both '%x|' 0 '0|'
both '%o|' 0 '0|'
both '%u|' 0 '0|'
both '%#.0o|' 0 '0|'
pc_pass

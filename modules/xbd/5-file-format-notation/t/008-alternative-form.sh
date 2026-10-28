# The # flag on o, x and X.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XBD.5-file-format-notation.008
. "$PC_CORE/pc.sh"
pc_begin XBD.5-file-format-notation.008
LC_ALL=POSIX
export LC_ALL
pc_test
both() {
	got=$(printf "$1" "$2" 2>/dev/null </dev/null)
	[ "$got" = "$3" ] || pc_fail "printf '$1' '$2' wrote '$got', expected '$3'"
	got=$(awk -v f="$1" -v a="$2" 'BEGIN { printf f, a }' 2>/dev/null </dev/null)
	[ "$got" = "$3" ] || pc_fail "awk printf \"$1\", \"$2\" wrote '$got', expected '$3'"
}
both '%#o|' 8 '010|'
both '%#o|' 64 '0100|'
both '%#o|' 1 '01|'
both '%#o|' 0 '0|'
both '%#x|' 255 '0xff|'
both '%#X|' 255 '0XFF|'
both '%#x|' 0 '0|'
both '%#X|' 0 '0|'
pc_pass

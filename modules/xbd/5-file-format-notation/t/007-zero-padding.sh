# The 0 flag pads with zeros after the sign, the space and the base prefix.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XBD.5-file-format-notation.007
. "$PC_CORE/pc.sh"
pc_begin XBD.5-file-format-notation.007
LC_ALL=POSIX
export LC_ALL
pc_test
both() {
	got=$(printf "$1" "$2" 2>/dev/null </dev/null)
	[ "$got" = "$3" ] || pc_fail "printf '$1' '$2' wrote '$got', expected '$3'"
	got=$(awk -v f="$1" -v a="$2" 'BEGIN { printf f, a }' 2>/dev/null </dev/null)
	[ "$got" = "$3" ] || pc_fail "awk printf \"$1\", \"$2\" wrote '$got', expected '$3'"
}
both '%05d|' 42 '00042|'
both '%05d|' -42 '-0042|'
both '%+05d|' 42 '+0042|'
both '% 05d|' 42 ' 0042|'
both '%06x|' 255 '0000ff|'
both '%#06x|' 255 '0x00ff|'
both '%#06X|' 255 '0X00FF|'
both '%06o|' 8 '000010|'
pc_pass

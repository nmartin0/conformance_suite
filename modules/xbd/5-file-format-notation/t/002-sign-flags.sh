# The + and space flags, alone and together.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XBD.5-file-format-notation.002
. "$PC_CORE/pc.sh"
pc_begin XBD.5-file-format-notation.002
LC_ALL=POSIX
export LC_ALL
pc_test
both() {
	got=$(printf "$1" "$2" 2>/dev/null </dev/null)
	[ "$got" = "$3" ] || pc_fail "printf '$1' '$2' wrote '$got', expected '$3'"
	got=$(awk -v f="$1" -v a="$2" 'BEGIN { printf f, a }' 2>/dev/null </dev/null)
	[ "$got" = "$3" ] || pc_fail "awk printf \"$1\", \"$2\" wrote '$got', expected '$3'"
}
both '%+d|' 7 '+7|'
both '%+d|' -7 '-7|'
both '% d|' 7 ' 7|'
both '% d|' -7 '-7|'
both '%+ d|' 7 '+7|'
both '% +d|' 7 '+7|'
both '% +d|' -7 '-7|'
pc_pass

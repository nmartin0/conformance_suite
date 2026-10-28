# With a precision, the 0 flag has no effect on d, i, o, u, x and X.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XBD.5-file-format-notation.006
. "$PC_CORE/pc.sh"
pc_begin XBD.5-file-format-notation.006
LC_ALL=POSIX
export LC_ALL
pc_test
both() {
	got=$(printf "$1" "$2" 2>/dev/null </dev/null)
	[ "$got" = "$3" ] || pc_fail "printf '$1' '$2' wrote '$got', expected '$3'"
	got=$(awk -v f="$1" -v a="$2" 'BEGIN { printf f, a }' 2>/dev/null </dev/null)
	[ "$got" = "$3" ] || pc_fail "awk printf \"$1\", \"$2\" wrote '$got', expected '$3'"
}
both '%05.3d|' 7 '  007|'
both '%08.5x|' 255 '   000ff|'
both '%06.2o|' 8 '    10|'
both '%05.3u|' 7 '  007|'
both '%05.3i|' -7 ' -007|'
pc_pass

# A field width pads on the left (right with -) and never truncates.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XBD.5-file-format-notation.001
. "$PC_CORE/pc.sh"
pc_begin XBD.5-file-format-notation.001
LC_ALL=POSIX
export LC_ALL
pc_test
# both <format> <argument> <expected output>: the printf utility and the awk
# printf statement must both write the expected text.
both() {
	got=$(printf "$1" "$2" 2>/dev/null </dev/null)
	[ "$got" = "$3" ] || pc_fail "printf '$1' '$2' wrote '$got', expected '$3'"
	got=$(awk -v f="$1" -v a="$2" 'BEGIN { printf f, a }' 2>/dev/null </dev/null)
	[ "$got" = "$3" ] || pc_fail "awk printf \"$1\", \"$2\" wrote '$got', expected '$3'"
}
both '%3d|' 12345 '12345|'
both '%5d|' 42 '   42|'
both '%-5d|' 42 '42   |'
both '%3c|' x '  x|'
both '%-3c|' x 'x  |'
both '%2s|' abc 'abc|'
both '%5s|' abc '  abc|'
both '%-5s|' abc 'abc  |'
pc_pass

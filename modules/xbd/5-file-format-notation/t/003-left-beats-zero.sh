# With both the - and 0 flags the 0 flag has no effect.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XBD.5-file-format-notation.003
. "$PC_CORE/pc.sh"
pc_begin XBD.5-file-format-notation.003
LC_ALL=POSIX
export LC_ALL
pc_test
both() {
	got=$(printf "$1" "$2" 2>/dev/null </dev/null)
	[ "$got" = "$3" ] || pc_fail "printf '$1' '$2' wrote '$got', expected '$3'"
	got=$(awk -v f="$1" -v a="$2" 'BEGIN { printf f, a }' 2>/dev/null </dev/null)
	[ "$got" = "$3" ] || pc_fail "awk printf \"$1\", \"$2\" wrote '$got', expected '$3'"
}
both '%-05d|' 42 '42   |'
both '%0-5d|' 42 '42   |'
both '%-06x|' 255 'ff    |'
pc_pass

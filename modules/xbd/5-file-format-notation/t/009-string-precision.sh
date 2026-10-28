# The precision of s is the most bytes written; a bare period means zero.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XBD.5-file-format-notation.009
. "$PC_CORE/pc.sh"
pc_begin XBD.5-file-format-notation.009
LC_ALL=POSIX
export LC_ALL
pc_test
both() {
	got=$(printf "$1" "$2" 2>/dev/null </dev/null)
	[ "$got" = "$3" ] || pc_fail "printf '$1' '$2' wrote '$got', expected '$3'"
	got=$(awk -v f="$1" -v a="$2" 'BEGIN { printf f, a }' 2>/dev/null </dev/null)
	[ "$got" = "$3" ] || pc_fail "awk printf \"$1\", \"$2\" wrote '$got', expected '$3'"
}
both '%.2s|' abc 'ab|'
both '%.5s|' abc 'abc|'
both '%.0s|' abc '|'
both '%.s|' abc '|'
both '%5.2s|' abc '   ab|'
both '%-5.1s|' abc 'a    |'
pc_pass

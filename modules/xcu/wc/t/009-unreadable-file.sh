# A file operand that cannot be read gives a nonzero status, and the other operands are still counted.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.wc.009
. "$PC_CORE/pc.sh"
pc_begin XCU.wc.009
LC_ALL=POSIX
export LC_ALL
NL='
'
printf 'a b\n' >g || pc_fail "cannot create input"
[ ! -e nosuchfile ] || pc_unresolved "nosuchfile exists"
pc_test
out=$(wc -l nosuchfile g 2>/dev/null </dev/null)
rc=$?
[ "$rc" -ne 0 ] || pc_fail "status 0 although a file could not be read"
# the entry for g is a line of the count and the name (blanks around them are not fixed)
found=
while IFS= read -r line; do
	set -- $line
	if [ "$#" -eq 2 ] && [ "$1" = 1 ] && [ "$2" = g ]; then found=1; fi
done <<EOF
$out
EOF
[ -n "$found" ] || pc_fail "the readable file was not counted"
pc_pass

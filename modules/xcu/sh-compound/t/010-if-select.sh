# if runs the then list when its condition succeeds, else tries each elif,
# else runs the else list.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.sh-compound.010
. "$PC_CORE/pc.sh"
pc_begin XCU.sh-compound.010
pc_test
if true; then r=1; fi
[ "$r" = 1 ] || pc_fail "then: '$r'"
if false; then r=1; else r=2; fi
[ "$r" = 2 ] || pc_fail "else: '$r'"
if false; then r=1; elif true; then r=3; else r=4; fi
[ "$r" = 3 ] || pc_fail "elif: '$r'"
if false; then r=1; elif false; then r=3; else r=4; fi
[ "$r" = 4 ] || pc_fail "else after elif: '$r'"
r=$(if true; then
	echo yes

	echo two
fi)
[ "$r" = "yes
two" ] || pc_fail "newlines in lists: '$r'"
pc_pass

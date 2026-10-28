# -I replacement string.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.xargs.010
. "$PC_CORE/pc.sh"
pc_begin XCU.xargs.010
LC_ALL=C; export LC_ALL
nl='
'
pc_test
printf 'a b\n  c\n' | xargs -I '{}' echo '<{}>' '{}' >out 2>/dev/null || pc_fail "xargs -I failed"
v=$(cat out; printf x)
v=${v%x}
[ "$v" = "<a b> a b${nl}<c> c$nl" ] || pc_fail "-I wrote '$v', expected '<a b> a b' then '<c> c'"
pc_pass

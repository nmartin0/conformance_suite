# -s SIZE.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.xargs.008
. "$PC_CORE/pc.sh"
pc_begin XCU.xargs.008
LC_ALL=C; export LC_ALL
nl='
'
pc_test
# echo (5) + aaaa (5) + bbbb (5) = 15 < 18; adding cccc makes 20.
printf 'aaaa bbbb cccc\n' | xargs -s 18 echo >out 2>/dev/null || pc_fail "xargs -s 18 failed"
v=$(cat out; printf x)
v=${v%x}
[ "$v" = "aaaa bbbb${nl}cccc$nl" ] || pc_fail "-s 18 wrote '$v', expected 'aaaa bbbb' then 'cccc'"
printf 'aaaa bbbb cccc\n' | xargs -s 1000 echo >out 2>/dev/null || pc_fail "xargs -s 1000 failed"
v=$(cat out; printf x)
v=${v%x}
[ "$v" = "aaaa bbbb cccc$nl" ] || pc_fail "-s 1000 wrote '$v', expected one line"
pc_pass

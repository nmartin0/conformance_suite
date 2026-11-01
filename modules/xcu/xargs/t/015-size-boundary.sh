# -s SIZE: the length is less than SIZE, not equal to it.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.xargs.016
# MUTANT    xargs.s_le
. "$PC_CORE/pc.sh"
pc_begin XCU.xargs.016
LC_ALL=C; export LC_ALL
nl='
'
pc_test
# echo (5) + aaaa (5) + bbbb (5) = 15: below 16, not below 15.
printf 'aaaa bbbb cccc\n' | xargs -s 16 echo >out 2>/dev/null || pc_fail "xargs -s 16 failed"
v=$(cat out; printf x)
v=${v%x}
[ "$v" = "aaaa bbbb${nl}cccc$nl" ] || pc_fail "-s 16 wrote '$v', expected 'aaaa bbbb' then 'cccc'"
printf 'aaaa bbbb cccc\n' | xargs -s 15 echo >out 2>/dev/null || pc_fail "xargs -s 15 failed"
v=$(cat out; printf x)
v=${v%x}
[ "$v" = "aaaa${nl}bbbb${nl}cccc$nl" ] || pc_fail "-s 15 wrote '$v', expected one argument per run"
pc_pass

# -n NUMBER.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.xargs.003
. "$PC_CORE/pc.sh"
pc_begin XCU.xargs.003
LC_ALL=C; export LC_ALL
nl='
'
pc_test
printf '1 2 3 4 5\n' | xargs -n 2 echo >out 2>/dev/null || pc_fail "xargs -n 2 failed"
v=$(cat out; printf x)
v=${v%x}
[ "$v" = "1 2${nl}3 4${nl}5$nl" ] || pc_fail "-n 2 wrote '$v'"
printf '1 2 3\n' | xargs -n 1 echo >out 2>/dev/null || pc_fail "xargs -n 1 failed"
v=$(cat out; printf x)
v=${v%x}
[ "$v" = "1${nl}2${nl}3$nl" ] || pc_fail "-n 1 wrote '$v'"
printf '1 2 3\n' | xargs -n 10 echo >out 2>/dev/null || pc_fail "xargs -n 10 failed"
v=$(cat out; printf x)
v=${v%x}
[ "$v" = "1 2 3$nl" ] || pc_fail "-n 10 wrote '$v'"
pc_pass

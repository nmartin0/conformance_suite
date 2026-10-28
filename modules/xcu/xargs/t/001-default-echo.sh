# Default utility is echo; initial arguments come first.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.xargs.001
. "$PC_CORE/pc.sh"
pc_begin XCU.xargs.001
LC_ALL=C; export LC_ALL
nl='
'
pc_test
printf 'a b\nc\n' | xargs >out 2>/dev/null || pc_fail "xargs without a utility failed"
v=$(cat out; printf x)
v=${v%x}
[ "$v" = "a b c$nl" ] || pc_fail "xargs without a utility wrote '$v', expected 'a b c'"
printf 'a\n' | xargs echo pre >out 2>/dev/null || pc_fail "xargs echo pre failed"
v=$(cat out; printf x)
v=${v%x}
[ "$v" = "pre a$nl" ] || pc_fail "xargs echo pre wrote '$v', expected 'pre a'"
pc_pass

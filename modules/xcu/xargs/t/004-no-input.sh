# Runs once without input, not at all with -r.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.xargs.004
. "$PC_CORE/pc.sh"
pc_begin XCU.xargs.004
LC_ALL=C; export LC_ALL
nl='
'
pc_test
xargs echo x </dev/null >out 2>/dev/null || pc_fail "xargs echo x with empty input failed"
v=$(cat out; printf x)
v=${v%x}
[ "$v" = "x$nl" ] || pc_fail "without -r and without input the utility ran '$v', expected exactly one run"
xargs -r echo x </dev/null >out 2>/dev/null || pc_fail "xargs -r with empty input failed"
[ -s out ] && pc_fail "xargs -r ran the utility although there was no input"
printf '\n\n' | xargs -r echo x >out 2>/dev/null || pc_fail "xargs -r with blank lines failed"
[ -s out ] && pc_fail "xargs -r ran the utility for blank lines only"
printf 'a\n' | xargs -r echo x >out 2>/dev/null || pc_fail "xargs -r with input failed"
v=$(cat out; printf x)
v=${v%x}
[ "$v" = "x a$nl" ] || pc_fail "xargs -r with input wrote '$v'"
pc_pass

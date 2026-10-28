# -E and the logical end of file.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.xargs.005
. "$PC_CORE/pc.sh"
pc_begin XCU.xargs.005
LC_ALL=C; export LC_ALL
nl='
'
pc_test
printf 'a b STOP c d\n' | xargs -E STOP echo >out 2>/dev/null || pc_fail "xargs -E STOP failed"
v=$(cat out; printf x)
v=${v%x}
[ "$v" = "a b$nl" ] || pc_fail "-E STOP wrote '$v', expected 'a b'"
printf 'a STOP\nb\n' | xargs -E STOP echo >out 2>/dev/null || pc_fail "xargs -E STOP over lines failed"
v=$(cat out; printf x)
v=${v%x}
[ "$v" = "a$nl" ] || pc_fail "-E STOP with later lines wrote '$v', expected 'a'"
printf 'a \042STOP\042 b\n' | xargs -E STOP echo >out 2>/dev/null || pc_fail "xargs -E STOP with quoted STOP failed"
v=$(cat out; printf x)
v=${v%x}
[ "$v" = "a$nl" ] || pc_fail "a quoted STOP did not end the input after quote processing: '$v'"
printf 'a _ b\n' | xargs -E '' echo >out 2>/dev/null || pc_fail "xargs -E '' failed"
v=$(cat out; printf x)
v=${v%x}
[ "$v" = "a _ b$nl" ] || pc_fail "-E '' wrote '$v', expected 'a _ b'"
pc_pass

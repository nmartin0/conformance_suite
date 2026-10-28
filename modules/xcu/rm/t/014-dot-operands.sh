# An operand whose last component is dot or dot-dot is diagnosed and nothing is removed.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.rm.014
. "$PC_CORE/pc.sh"
pc_begin XCU.rm.014
mkdir -p p/w/sub || pc_fail "cannot create directories"
printf x >p/w/f || pc_fail "cannot create p/w/f"
printf x >p/w/sub/g || pc_fail "cannot create p/w/sub/g"
cd p/w || pc_fail "cannot enter p/w"
pc_test
rm -r . >out 2>err </dev/null
rc=$?
[ "$rc" -ne 0 ] || pc_fail "rm -r . exited 0"
[ -s err ] || pc_fail "rm -r . gave no diagnostic"
[ -f f ] && [ -f sub/g ] || pc_fail "rm -r . removed files"
rm -r .. >out 2>err </dev/null
rc=$?
[ "$rc" -ne 0 ] || pc_fail "rm -r .. exited 0"
[ -s err ] || pc_fail "rm -r .. gave no diagnostic"
[ -f f ] && [ -f sub/g ] || pc_fail "rm -r .. removed files"
rm -r sub/. >out 2>err </dev/null
rc=$?
[ "$rc" -ne 0 ] || pc_fail "rm -r sub/. exited 0"
[ -f sub/g ] || pc_fail "rm -r sub/. removed files"
pc_pass

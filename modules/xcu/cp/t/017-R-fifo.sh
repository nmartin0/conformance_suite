# cp -R recreates a FIFO in the hierarchy as a FIFO.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.cp.017
. "$PC_CORE/pc.sh"
pc_begin XCU.cp.017
mkdir src || pc_fail "cannot create src"
mkfifo src/p || pc_fail "cannot create fifo"
pc_test
cp -R src dst >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "cp -R exited with status $rc"
[ -p dst/p ] || pc_fail "dst/p is not a FIFO"
pc_pass

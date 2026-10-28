# rm removes each named file and leaves others alone; it writes nothing to standard output without -v.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.rm.001
. "$PC_CORE/pc.sh"
pc_begin XCU.rm.001
printf a >a || pc_fail "cannot create a"
printf b >b || pc_fail "cannot create b"
printf c >c || pc_fail "cannot create c"
pc_test
rm a b >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "rm exited with status $rc"
[ ! -e a ] && [ ! -e b ] || pc_fail "a named file still exists"
[ -f c ] || pc_fail "c was removed"
[ ! -s out ] || pc_fail "rm wrote to standard output"
pc_pass

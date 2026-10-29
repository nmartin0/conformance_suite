# rmdir removes each named empty directory, in order, with no output.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.rmdir.001
. "$PC_CORE/pc.sh"
pc_begin XCU.rmdir.001
mkdir d1 d2 || pc_fail "cannot create directories"
[ -d d1 ] && [ -d d2 ] || pc_fail "the directories were not created"
pc_test
rmdir d1 d2 >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "rmdir exited with status $rc"
[ ! -e d1 ] && [ ! -e d2 ] || pc_fail "a directory still exists"
[ ! -s out ] || pc_fail "rmdir wrote to standard output"
pc_pass

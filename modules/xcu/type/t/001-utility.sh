# type for a utility in PATH succeeds and writes something.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.type.001
. "$PC_CORE/pc.sh"
pc_begin XCU.type.001
mkdir bin || pc_fail "cannot make a directory"
printf 'exit 0\n' >bin/pcty1 || pc_fail "cannot write a script"
chmod +x bin/pcty1 || pc_fail "cannot make the script executable"
PATH=$PWD/bin:$PATH
pc_test
type pcty1 >out 2>/dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "type pcty1 exited $rc"
[ -s out ] || pc_fail "type wrote nothing to standard output"
pc_pass

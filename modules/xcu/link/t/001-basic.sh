# link file1 file2 makes file2 a second name for file1, with no output.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.link.001
#
# A user may need privileges to run link, so the test first makes sure
# that a hard link can be made with ln.
. "$PC_CORE/pc.sh"
pc_begin XCU.link.001
printf 'x\n' >a || pc_fail "cannot create a"
ln a probe || pc_unresolved "setup: hard links are not available here"
pc_test
link a b >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "link exited with status $rc"
[ a -ef b ] || pc_fail "b is not a name for a"
set -- $(ls -ld a)
[ "$2" = 3 ] || pc_fail "link count is $2, expected 3"
[ ! -s out ] || pc_fail "link wrote to standard output"
pc_pass

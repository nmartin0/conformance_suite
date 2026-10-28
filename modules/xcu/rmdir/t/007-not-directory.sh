# rmdir of a file that is not a directory fails and leaves the file.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.rmdir.007
. "$PC_CORE/pc.sh"
pc_begin XCU.rmdir.007
printf x >f || pc_fail "cannot create f"
pc_test
rmdir f >out 2>err </dev/null
rc=$?
[ "$rc" -ne 0 ] || pc_fail "rmdir exited 0 for a regular file"
[ -f f ] || pc_fail "f is gone"
pc_pass

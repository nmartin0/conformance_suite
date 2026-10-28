# mkfifo on a name that exists fails with a diagnostic and leaves the file alone.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.mkfifo.005
. "$PC_CORE/pc.sh"
pc_begin XCU.mkfifo.005
printf 'orig\n' >f || pc_fail "cannot create f"
pc_test
mkfifo f >out 2>err </dev/null
rc=$?
[ "$rc" -ne 0 ] || pc_fail "mkfifo exited 0 for an existing file"
[ -s err ] || pc_fail "no diagnostic"
[ -f f ] || pc_fail "f is no longer a regular file"
pc_pass

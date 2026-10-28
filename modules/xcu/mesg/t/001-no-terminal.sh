# mesg with no terminal on the standard streams.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.mesg.001
. "$PC_CORE/pc.sh"
pc_begin XCU.mesg.001
LC_ALL=C; export LC_ALL
pc_test
mesg </dev/null >/dev/null 2>&1
rc=$?
[ "$rc" -gt 1 ] || pc_fail "mesg with no terminal exited with status $rc, expected above 1"
pc_pass

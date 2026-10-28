# sh with a command_file that does not exist exits with status 127.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.sh.008
. "$PC_CORE/pc.sh"
pc_begin XCU.sh.008
pc_test
${PC_SH:-sh} pc.nosuchfile </dev/null 2>pc.err
rc=$?
[ "$rc" -eq 127 ] || pc_fail "status $rc, expected 127"
${PC_SH:-sh} ./pc.nodir/pc.nosuchfile </dev/null 2>/dev/null
rc=$?
[ "$rc" -eq 127 ] || pc_fail "with a slash: status $rc, expected 127"
pc_pass

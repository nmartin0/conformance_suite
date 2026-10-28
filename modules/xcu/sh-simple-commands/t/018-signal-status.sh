# A command ended by a signal gives a status above 128.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.sh-simple-commands.018
. "$PC_CORE/pc.sh"
pc_begin XCU.sh-simple-commands.018
pc_test
${PC_SH:-sh} -c 'kill -s KILL $$' </dev/null
rc=$?
[ "$rc" -gt 128 ] || pc_fail "status $rc, expected above 128"
${PC_SH:-sh} -c 'kill -s TERM $$' </dev/null
rc=$?
[ "$rc" -gt 128 ] || pc_fail "status $rc after TERM, expected above 128"
pc_pass

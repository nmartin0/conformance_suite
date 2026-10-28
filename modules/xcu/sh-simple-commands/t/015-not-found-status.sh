# A command that is not found gives status 127 and a message on standard
# error.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.sh-simple-commands.015
. "$PC_CORE/pc.sh"
pc_begin XCU.sh-simple-commands.015
printf '%s\n' 'pc-no-such-utility-xyz' 'echo "rc=$?"' >pc.s || pc_unresolved "write"
pc_test
out=$(${PC_SH:-sh} pc.s 2>pc.err </dev/null)
rc=$?
if [ "$rc" -eq 0 ]; then
	[ "$out" = "rc=127" ] || pc_fail "shell went on with out='$out', expected rc=127"
else
	[ "$rc" -eq 127 ] && [ -z "$out" ] || pc_fail "shell exited with status $rc and output '$out'; expected 127"
fi
[ -s pc.err ] || pc_fail "no diagnostic on standard error"
pc_pass

# A file that exists but cannot be executed (no execute permission) gives
# status 126.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.sh-simple-commands.016
. "$PC_CORE/pc.sh"
pc_begin XCU.sh-simple-commands.016
printf '%s\n' ':' >pc.ne || pc_unresolved "write"
chmod 644 pc.ne || pc_unresolved "chmod"
printf '%s\n' './pc.ne' 'echo "rc=$?"' >pc.s || pc_unresolved "write"
pc_test
out=$(${PC_SH:-sh} pc.s 2>pc.err </dev/null)
rc=$?
if [ "$rc" -eq 0 ]; then
	[ "$out" = "rc=126" ] || pc_fail "shell went on with out='$out', expected rc=126"
else
	[ "$rc" -eq 126 ] && [ -z "$out" ] || pc_fail "shell exited with status $rc and output '$out'; expected 126"
fi
pc_pass

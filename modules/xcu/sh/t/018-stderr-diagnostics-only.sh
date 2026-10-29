# sh writes to standard error only diagnostics: running good commands from a
# string, a file or standard input leaves standard error empty.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.sh.018
. "$PC_CORE/pc.sh"
pc_begin XCU.sh.018
printf '%s\n' 'echo hi' >pc.cf || pc_unresolved "write"
pc_test
# each run must also have run the command (it wrote hi), so that an empty
# standard error is not just a shell that did nothing
r1=$(${PC_SH:-sh} -c 'echo hi' </dev/null 2>pc.e1)
r2=$(${PC_SH:-sh} pc.cf </dev/null 2>pc.e2)
r3=$(${PC_SH:-sh} <pc.cf 2>pc.e3)
for r in "$r1" "$r2" "$r3"; do
	[ "$r" = hi ] || pc_fail "a good command did not run (standard output '$r')"
done
for f in pc.e1 pc.e2 pc.e3; do
	[ -s "$f" ] && pc_fail "$f: standard error is not empty"
done
pc_pass

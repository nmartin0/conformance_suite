# nl reports an option that lacks its argument on standard error with a status above zero, and an unreadable file with a status above zero.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.nl.011
# MUTANT    nl.optarg_accepted
# MUTANT    nl.optarg_nodiag
# MUTANT    nl.optarg_status0
# MUTANT    nl.optarg_stdout
# MUTANT    nl.missing_status0
# VARIANT   nl.status2
# VARIANT   nl.status255
. "$PC_CORE/pc.sh"
pc_begin XCU.nl.011
LC_ALL=POSIX
export LC_ALL
pc_test
for o in b d f h i l n s v w; do
	nl -$o >out 2>err </dev/null
	rc=$?
	[ -f err ] || pc_fail "no standard error was captured"
	[ "$rc" -gt 0 ] || pc_fail "-$o without its argument gave status 0"
	[ -s err ] || pc_fail "-$o without its argument gave no diagnostic on standard error"
done
nl no-such-file-here >out 2>/dev/null </dev/null
rc=$?
[ "$rc" -gt 0 ] || pc_fail "a nonexistent file operand gave status 0"
pc_pass

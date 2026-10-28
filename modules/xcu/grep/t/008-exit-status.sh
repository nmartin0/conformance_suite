# grep exits 0 when a line is selected, 1 when none is, above 1 on error; -s hides messages for unreadable files and -q gives 0 despite an error.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.grep.008
. "$PC_CORE/pc.sh"
pc_begin XCU.grep.008
LC_ALL=POSIX
export LC_ALL
NL='
'
printf 'a\n' >f1 || pc_fail "cannot create input"
[ ! -e nosuchfile ] || pc_unresolved "nosuchfile exists"
pc_test
grep a f1 >/dev/null 2>&1 </dev/null
[ $? -eq 0 ] || pc_fail "status was not 0 for a match"
grep b f1 >/dev/null 2>&1 </dev/null
[ $? -eq 1 ] || pc_fail "status was not 1 for no match"
grep a nosuchfile >/dev/null 2>&1 </dev/null
rc=$?
[ "$rc" -gt 1 ] || pc_fail "status was not above 1 for a missing file"
err=$(grep a nosuchfile 2>&1 >/dev/null </dev/null)
[ -n "$err" ] || pc_fail "no diagnostic for a missing file"
err=$(grep -s a nosuchfile 2>&1 >/dev/null </dev/null)
[ -z "$err" ] || pc_fail "-s did not suppress the missing-file diagnostic"
grep -s a nosuchfile >/dev/null 2>&1 </dev/null
rc=$?
[ "$rc" -gt 1 ] || pc_fail "-s changed the failure status"
grep -q a nosuchfile f1 >/dev/null 2>&1 </dev/null
[ $? -eq 0 ] || pc_fail "-q did not give status 0 after a match despite an earlier error"
pc_pass

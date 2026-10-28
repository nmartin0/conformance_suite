# diff writes nothing and exits 0 for identical files, exits 1 when it writes differences, and exits above 1 on an error.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.diff.002
. "$PC_CORE/pc.sh"
pc_begin XCU.diff.002
LC_ALL=POSIX
export LC_ALL
NL='
'
mkf() {
	n=$1
	shift
	: >"$n" || pc_unresolved "cannot create input"
	for l in "$@"; do printf '%s\n' "$l" >>"$n"; done
}
mkf same1 a b c
mkf same2 a b c
mkf other a b d
: >empty1
: >empty2
mkf one x
[ ! -e nosuchfile ] || pc_unresolved "nosuchfile exists"
pc_test
got=$(diff same1 same2 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "rc=0" ] || pc_fail "identical files gave output or a status other than 0"
got=$(diff same1 same1 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "rc=0" ] || pc_fail "a file compared with itself gave output or a status other than 0"
got=$(diff empty1 empty2 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "rc=0" ] || pc_fail "two empty files gave output or a status other than 0"
diff same1 other >/dev/null 2>&1 </dev/null
[ $? -eq 1 ] || pc_fail "differing files did not give status 1"
got=$(diff empty1 one 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "0a1${NL}> x${NL}rc=1" ] || pc_fail "an empty file against a one-line file wrong"
got=$(diff one empty1 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "1d0${NL}< x${NL}rc=1" ] || pc_fail "a one-line file against an empty file wrong"
out=$(diff same1 nosuchfile 2>/dev/null </dev/null)
rc=$?
[ "$rc" -gt 1 ] || pc_fail "a missing second file did not give a status above 1"
[ -z "$out" ] || pc_fail "output was written for a missing file"
diff nosuchfile same1 >/dev/null 2>&1 </dev/null
[ $? -gt 1 ] || pc_fail "a missing first file did not give a status above 1"
err=$(diff same1 nosuchfile 2>&1 >/dev/null </dev/null)
[ -n "$err" ] || pc_fail "no diagnostic for a missing file"
pc_pass

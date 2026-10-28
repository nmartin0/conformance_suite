# pax: errors on single files do not stop the run
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.pax.031
# A file operand that does not exist, or a member that cannot be created,
# gives a diagnostic and a nonzero status but processing continues with the
# other files; a ustar hard-link member extracted alone either gets its data
# or gives a nonzero status and a diagnostic that names the member holding
# the data.
. "$PC_CORE/pc.sh"
pc_begin XCU.pax.031
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
printf '1\n' >f1 && printf '2\n' >f2 || pc_unresolved "setup: cannot write files"
printf 'payload\n' >data1 && ln data1 lnk2 || pc_unresolved "setup: cannot make the hard link"
pax -w f1 f2 >a 2>/dev/null </dev/null || pc_unresolved "setup: pax -w failed"
pax -w -x ustar data1 lnk2 >h 2>/dev/null </dev/null || pc_unresolved "setup: pax -w of the linked files failed"
pc_test
pax -w f1 nosuchfile f2 >b 2>err </dev/null
rc=$?
[ "$rc" -ne 0 ] || pc_fail "a missing file operand gave exit status 0"
[ -s err ] || pc_fail "no diagnostic for the missing file operand"
got=$(pax <b 2>/dev/null | sort | tr '\n' ' ')
[ "$got" = 'f1 f2 ' ] || pc_fail "after a missing operand the archive lists '$got', expected f1 and f2"
rm -f f1 f2
mkdir f1 || pc_unresolved "setup: mkdir f1 failed"
printf 'in the way\n' >f1/x || pc_unresolved "setup: cannot write f1/x"
pax -r <a >o 2>err
rc=$?
[ "$rc" -ne 0 ] || pc_fail "a member that cannot be created gave exit status 0"
[ -s err ] || pc_fail "no diagnostic for the member that cannot be created"
[ "$(cat f2 2>/dev/null)" = 2 ] || pc_fail "f2 was not extracted after the failure on f1"
rm -f data1 lnk2
pax -r lnk2 <h >o 2>err
rc=$?
if [ "$rc" -eq 0 ]; then
	[ "$(cat lnk2 2>/dev/null)" = payload ] || pc_fail "status 0 but lnk2 does not hold the data of the archived file"
else
	grep data1 err >/dev/null || pc_fail "status $rc but the diagnostic does not name data1, the member that holds the data"
fi
pc_pass

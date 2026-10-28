# -h and -z
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.admin.007
# -h passes an intact file silently, reports a damaged one, and -z
# recomputes the checksum so that -h passes again.
. "$PC_CORE/pc.sh"
pc_begin XCU.admin.007
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
printf 'one\ntwo\nthree\n' >f || pc_unresolved "setup: cannot write f"
admin -if s.f </dev/null >setup.out 2>&1 || pc_unresolved "setup: admin could not create s.f"
cp s.f t.f || pc_unresolved "setup: cannot copy"
sed 's/one/ONE/' t.f >u.f || pc_unresolved "setup: sed failed"
cmp t.f u.f >/dev/null 2>&1 && pc_unresolved "setup: the damaged copy is identical"
mv u.f s.bad || pc_unresolved "setup: cannot rename"
pc_test
admin -h s.f </dev/null >out 2>err
rc=$?
[ "$rc" -eq 0 ] || pc_fail "admin -h on an intact file: exit status $rc"
if [ -s err ] || [ -s out ]; then
	pc_fail "admin -h wrote a diagnostic for an intact file"
fi
admin -h s.bad </dev/null >out 2>err
if [ ! -s err ] && [ ! -s out ]; then
	pc_fail "admin -h wrote no diagnostic for a damaged file"
fi
admin -z s.bad </dev/null >out 2>err || pc_fail "admin -z: nonzero status"
admin -h s.bad </dev/null >out 2>err
rc=$?
[ "$rc" -eq 0 ] || pc_fail "admin -h after -z: exit status $rc"
[ -s err ] && pc_fail "admin -h after -z still wrote a diagnostic: $(cat err)"
pc_pass

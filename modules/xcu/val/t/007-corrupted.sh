# val sets 0x20 for a modified SCCS file
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.val.007
. "$PC_CORE/pc.sh"
pc_begin XCU.val.007
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
for u in get admin prs delta unget val; do command -v $u >/dev/null 2>&1 || pc_notinuse "$u is not available"; done
printf 'one\ntwo\nthree\n' >f || pc_unresolved "setup: cannot write f"
admin -if s.f </dev/null >setup.out 2>&1 || pc_unresolved "setup: admin"
cp s.f t.f || pc_unresolved "setup: cp"
sed 's/one/ONE/' t.f >s.bad || pc_unresolved "setup: sed"
cmp t.f s.bad >/dev/null 2>&1 && pc_unresolved "setup: the damaged copy is identical"
val s.f </dev/null >out 2>err || pc_unresolved "setup: val of the intact file failed"
# The page does not define "corrupted"; the checksum is what it does define (admin -h).
admin -h s.f </dev/null >out 2>err || pc_unresolved "setup: admin -h of the intact file failed"
admin -h s.bad </dev/null >out 2>err
if [ "$?" -eq 0 ] && [ ! -s out ] && [ ! -s err ]; then
	pc_unresolved "setup: the checksum check of admin -h does not flag the damaged copy"
fi
pc_test
val s.bad </dev/null >out 2>err
rc=$?
[ $((rc & 32)) -ne 0 ] || pc_fail "a modified file gave status $rc without 0x20"
pc_pass

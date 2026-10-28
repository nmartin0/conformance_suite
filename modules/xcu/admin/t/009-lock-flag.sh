# admin -fl locks releases against get -e; -dl unlocks
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.admin.009
. "$PC_CORE/pc.sh"
pc_begin XCU.admin.009
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
for u in get admin prs; do command -v $u >/dev/null 2>&1 || pc_notinuse "$u is not available"; done
printf 'one\n' >f || pc_unresolved "setup: cannot write f"
admin -if s.f </dev/null >setup.out 2>&1 || pc_unresolved "setup: admin"
rm -f f
get -e -s s.f </dev/null >setup.out 2>&1 || pc_unresolved "setup: get -e on an unlocked file"
rm -f f p.f
pc_test
admin -fl1 s.f </dev/null >out 2>err || pc_fail "admin -fl1: nonzero status: $(cat err)"
get -e -s s.f </dev/null >out 2>err && pc_fail "get -e of the locked release 1 succeeded"
rm -f f p.f
admin -dl1 s.f </dev/null >out 2>err || pc_fail "admin -dl1: nonzero status: $(cat err)"
get -e -s s.f </dev/null >out 2>err || pc_fail "get -e after -dl1 failed: $(cat err)"
rm -f f p.f
admin -fla s.f </dev/null >out 2>err || pc_fail "admin -fla: nonzero status: $(cat err)"
get -e -s s.f </dev/null >out 2>err && pc_fail "get -e succeeded although all releases are locked"
rm -f f p.f
admin -dla s.f </dev/null >out 2>err || pc_fail "admin -dla: nonzero status: $(cat err)"
get -e -s s.f </dev/null >out 2>err || pc_fail "get -e after -dla failed: $(cat err)"
pc_pass

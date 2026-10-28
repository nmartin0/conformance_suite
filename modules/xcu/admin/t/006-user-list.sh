# -a and -e
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.admin.006
# -a adds login names to the list of users who may make deltas and -e
# removes them.
. "$PC_CORE/pc.sh"
pc_begin XCU.admin.006
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
command -v prs >/dev/null 2>&1 || pc_unresolved "setup: prs is not available"
printf 'x\n' >f || pc_unresolved "setup: cannot write f"
me=$(id -un) || pc_unresolved "setup: cannot find the login name"
admin -if s.f </dev/null >setup.out 2>&1 || pc_unresolved "setup: admin could not create s.f"
pc_test
admin -a "$me" -a bobx -a carolx s.f </dev/null >out 2>err || pc_fail "admin -a: nonzero status"
prs -r -d':UN:' s.f </dev/null >un 2>/dev/null
grep -x bobx un >/dev/null || pc_fail "bobx is not in the user list: $(cat un)"
grep -x carolx un >/dev/null || pc_fail "carolx is not in the user list: $(cat un)"
admin -e bobx s.f </dev/null >out 2>err || pc_fail "admin -e: nonzero status"
prs -r -d':UN:' s.f </dev/null >un 2>/dev/null
grep -x bobx un >/dev/null && pc_fail "bobx is still in the user list after -e"
grep -x carolx un >/dev/null || pc_fail "carolx was lost by -e bobx"
pc_pass

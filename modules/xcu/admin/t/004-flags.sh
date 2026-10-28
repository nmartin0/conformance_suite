# -f sets flags and -d removes them
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.admin.004
# The m, t, q, b, j, c, f, n, d and i flags set with -f are reflected by
# prs, the default module name is the file name without s., and -d removes b
# and j.
. "$PC_CORE/pc.sh"
pc_begin XCU.admin.004
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
command -v prs >/dev/null 2>&1 || pc_unresolved "setup: prs is not available"
printf 'x\n' >f || pc_unresolved "setup: cannot write f"
admin -if s.f </dev/null >setup.out 2>&1 || pc_unresolved "setup: admin could not create s.f"
pc_test
got=$(prs -r -d':M:' s.f </dev/null 2>/dev/null)
[ "$got" = f ] || pc_fail "the default module name is '$got', expected f"
admin -fmmymod -ftmytype -fqqtext -fb -fj -fc5 -ff2 -fn -fd1.1 -fi s.f </dev/null >out 2>err || pc_fail "admin -f: nonzero status"
got=$(prs -r -d':M:|:Y:|:Q:|:BF:|:J:|:ND:|:Ds:|:KF:' s.f </dev/null 2>/dev/null)
[ "$got" = 'mymod|mytype|qtext|yes|yes|yes|1.1|yes' ] || pc_fail "flags read back as '$got'"
got=$(prs -r -d':CB:|:FB:' s.f </dev/null 2>/dev/null)
printf '%s\n' "$got" | awk -F'|' '{ ok = ($1 + 0 == 5 && $2 + 0 == 2) } END { exit !ok }' || pc_fail "the ceiling and floor read back as '$got', expected 5 and 2"
admin -db -dj s.f </dev/null >out 2>err || pc_fail "admin -d: nonzero status"
got=$(prs -r -d':BF:|:J:|:M:' s.f </dev/null 2>/dev/null)
[ "$got" = 'no|no|mymod' ] || pc_fail "after -db -dj the flags read back as '$got'"
pc_pass

# admin -m with and without the v flag
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.admin.012
. "$PC_CORE/pc.sh"
pc_begin XCU.admin.012
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
for u in get admin prs; do command -v $u >/dev/null 2>&1 || pc_notinuse "$u is not available"; done
printf 'one\n' >f || pc_unresolved "setup: cannot write f"
pc_test
admin -if -fv -m'mr7 mr8' s.f </dev/null >out 2>err || pc_fail "admin -fv -m on creation: nonzero status: $(cat err)"
got=$(prs -r1.1 -d':MR:' s.f </dev/null 2>/dev/null)
case $got in *mr7*mr8*) ;; *) pc_fail "the MR numbers read back as '$got'" ;; esac
printf 'two\n' >g || pc_unresolved "cannot write g"
admin -ig -m'mr9' s.g </dev/null >out 2>err
[ -s err ] || pc_fail "-m without the v flag wrote no diagnostic"
pc_pass

# delta -m with an MR validation program
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.delta.009
. "$PC_CORE/pc.sh"
pc_begin XCU.delta.009
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
for u in get admin delta prs; do command -v $u >/dev/null 2>&1 || pc_notinuse "$u is not available"; done
pc_here=$(pwd) || pc_unresolved "cannot find the work directory"
cat >good.sh <<'IN'
#!/bin/sh
echo "$@" >"${PC_MRLOG:-/dev/null}"
exit 0
IN
cat >bad.sh <<'IN'
#!/bin/sh
exit 1
IN
chmod 755 good.sh bad.sh || pc_unresolved "setup: chmod"
printf 'one\n' >f || pc_unresolved "setup: cannot write f"
admin -if s.f </dev/null >setup.out 2>&1 || pc_unresolved "setup: admin"
admin -fv"$pc_here/good.sh" s.f </dev/null >setup.out 2>&1 || pc_unresolved "setup: admin -fv"
get -e -s s.f </dev/null >setup.out 2>&1 || pc_unresolved "setup: get -e"
printf 'one\ntwo\n' >f
pc_test
PC_MRLOG=$pc_here/mr.log; export PC_MRLOG
delta -s -m'a1 b2' -y'with MRs' s.f </dev/null >out 2>err || pc_fail "delta with a validator that exits 0: nonzero status: $(cat err)"
unset PC_MRLOG
got=$(prs -r1.2 -d':MR:' s.f </dev/null 2>/dev/null)
case $got in *a1*b2*) ;; *) pc_fail "the MR numbers were not recorded: '$got'" ;; esac
get -e -s s.f </dev/null >setup.out 2>&1 || pc_unresolved "setup: second get -e"
printf 'one\ntwo\nthree\n' >f
admin -dv s.f </dev/null >setup.out 2>&1 || pc_unresolved "setup: admin -dv"
admin -fv"$pc_here/bad.sh" s.f </dev/null >setup.out 2>&1 || pc_unresolved "setup: admin -fv bad"
delta -s -m'c3' -y'bad MR' s.f </dev/null >out 2>err
[ "$?" -ne 0 ] || pc_fail "delta succeeded although the validator exited non-zero"
get -s -p -r1.3 s.f </dev/null >got 2>/dev/null && pc_fail "a delta 1.3 was added although validation failed"
pc_pass

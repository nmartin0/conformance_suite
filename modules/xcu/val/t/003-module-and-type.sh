# -m and -y
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.val.003
# A -m name that is not the module name sets 0x01 and a -y type that is not
# the type flag sets 0x02, matching values give 0.
. "$PC_CORE/pc.sh"
pc_begin XCU.val.003
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
printf 'x\n' >f || pc_unresolved "setup: cannot write f"
admin -ftmytype -if s.f </dev/null >setup.out 2>&1 || pc_unresolved "setup: admin could not create s.f"
pc_test
val -m f -y mytype s.f </dev/null >out 2>err
rc=$?
[ "$rc" -eq 0 ] || pc_fail "matching -m and -y: exit status $rc"
val -s -m wrong s.f </dev/null >out 2>err
rc=$?
[ $((rc & 1)) -ne 0 ] || pc_fail "a wrong -m name: status $rc lacks 0x01"
val -s -y wrongtype s.f </dev/null >out 2>err
rc=$?
[ $((rc & 2)) -ne 0 ] || pc_fail "a wrong -y type: status $rc lacks 0x02"
pc_pass

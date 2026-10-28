# codes from several files are ORed
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.val.005
# Over several files the status is the OR of the individual codes.
. "$PC_CORE/pc.sh"
pc_begin XCU.val.005
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
printf 'one\ntwo\nthree\n' >f || pc_unresolved "setup: cannot write f"
admin -if s.f </dev/null >setup.out 2>&1 || pc_unresolved "setup: admin could not create s.f"
rm -f f
pc_test
val -s -m wrong s.f nosuchfile </dev/null >out 2>err
rc=$?
[ $((rc & 1)) -ne 0 ] || pc_fail "status $rc lacks 0x01 from the module name mismatch"
[ $((rc & 16)) -ne 0 ] || pc_fail "status $rc lacks 0x10 from the missing file"
pc_pass

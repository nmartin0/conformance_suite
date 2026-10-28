# -r
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.val.002
# -r of an existing SID passes, a nonexistent SID sets 0x04, and an
# ambiguous or invalid SID sets 0x08.
. "$PC_CORE/pc.sh"
pc_begin XCU.val.002
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
printf 'one\ntwo\nthree\n' >f || pc_unresolved "setup: cannot write f"
admin -if s.f </dev/null >setup.out 2>&1 || pc_unresolved "setup: admin could not create s.f"
rm -f f
pc_test
val -r 1.1 s.f </dev/null >out 2>err
rc=$?
[ "$rc" -eq 0 ] || pc_fail "val -r 1.1 on an existing SID: exit status $rc"
val -s -r 1.2 s.f </dev/null >out 2>err
rc=$?
[ $((rc & 4)) -ne 0 ] || pc_fail "val -r 1.2 (nonexistent): status $rc lacks 0x04"
val -s -r 1.0 s.f </dev/null >out 2>err
rc=$?
[ $((rc & 8)) -ne 0 ] || pc_fail "val -r 1.0 (invalid): status $rc lacks 0x08"
val -s -r 1 s.f </dev/null >out 2>err
rc=$?
[ $((rc & 8)) -ne 0 ] || pc_fail "val -r 1 (ambiguous): status $rc lacks 0x08"
pc_pass

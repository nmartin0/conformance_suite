# not an SCCS file, no file, -s and message format
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.val.004
# A file that is not SCCS sets 0x10, no file operand sets 0x80, an unknown
# option sets 0x40, messages start with the pathname and -s silences them.
. "$PC_CORE/pc.sh"
pc_begin XCU.val.004
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
printf 'one\ntwo\nthree\n' >f || pc_unresolved "setup: cannot write f"
admin -if s.f </dev/null >setup.out 2>&1 || pc_unresolved "setup: admin could not create s.f"
rm -f f
printf 'plain text\n' >notsccs || pc_unresolved "setup: cannot write notsccs"
pc_test
val notsccs </dev/null >out 2>err
rc=$?
[ $((rc & 16)) -ne 0 ] || pc_fail "a non-SCCS file: status $rc lacks 0x10"
grep '^notsccs: ' out >/dev/null || pc_fail "the message does not start with the pathname: $(cat out)"
val -s notsccs </dev/null >out 2>err
rc=$?
[ $((rc & 16)) -ne 0 ] || pc_fail "val -s: status $rc lacks 0x10"
[ -s out ] && pc_fail "-s did not silence the message: $(cat out)"
val -s </dev/null >out 2>err
rc=$?
[ $((rc & 128)) -ne 0 ] || pc_fail "no file operand: status $rc lacks 0x80"
val -Z s.f </dev/null >out 2>err
rc=$?
[ $((rc & 64)) -ne 0 ] || pc_fail "an unknown option: status $rc lacks 0x40"
pc_pass

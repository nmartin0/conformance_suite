# get with several files, a directory and "-" as operands
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.get.010
. "$PC_CORE/pc.sh"
pc_begin XCU.get.010
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
command -v get >/dev/null 2>&1 || pc_notinuse "get is not available"
command -v admin >/dev/null 2>&1 || pc_notinuse "admin is not available"
command -v delta >/dev/null 2>&1 || pc_notinuse "delta is not available"
mkdir d || pc_unresolved "setup: cannot make d"
printf 'one\n' >a || pc_unresolved "setup: cannot write a"
printf 'two\nthree\n' >b || pc_unresolved "setup: cannot write b"
admin -ia d/s.a </dev/null >setup.out 2>&1 || pc_unresolved "setup: admin d/s.a"
admin -ib d/s.b </dev/null >setup.out 2>&1 || pc_unresolved "setup: admin d/s.b"
printf 'x\n' >d/junk || pc_unresolved "setup: cannot write d/junk"
rm -f a b
pc_test
# several named files
get d/s.a d/s.b </dev/null >out 2>err || pc_fail "get of two files: nonzero status: $(cat err)"
grep -x 'd/s\.a:' out >/dev/null || pc_fail "no heading for d/s.a: $(cat out)"
grep -x 'd/s\.b:' out >/dev/null || pc_fail "no heading for d/s.b: $(cat out)"
grep -x '2 lines' out >/dev/null || pc_fail "no line count for s.b: $(cat out)"
[ "$(cat a)" = one ] || pc_fail "g-file a is wrong"
[ "$(cat b | wc -l | tr -d ' ')" = 2 ] || pc_fail "g-file b is wrong"
rm -f a b
# a directory
get d </dev/null >out 2>err || pc_fail "get of a directory: nonzero status: $(cat err)"
[ -f a ] && [ -f b ] || pc_fail "the directory operand did not process both SCCS files"
[ -s err ] && pc_fail "non-SCCS file produced a diagnostic: $(cat err)"
grep 's\.a:$' out >/dev/null || pc_fail "no heading for s.a with a directory: $(cat out)"
rm -f a b
# standard input
printf 'd/s.a\nd/s.b\n' | get - >out 2>err || pc_fail "get -: nonzero status: $(cat err)"
[ -f a ] && [ -f b ] || pc_fail "get - did not process the names read from standard input"
grep 'd/s\.b:$' out >/dev/null || pc_fail "no heading for d/s.b with -: $(cat out)"
pc_pass

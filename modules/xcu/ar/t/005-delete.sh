# ar -d deletes members
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ar.005
# "ar -d a f2" removes f2 and keeps the others in their order; with -v it
# writes "d - f2" to standard output.
. "$PC_CORE/pc.sh"
pc_begin XCU.ar.005
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
printf 'one\n' >f1; printf 'two\n' >f2; printf 'three\n' >f3; printf 'four\n' >f4; printf 'twenty\n' >f20
ar -rc a f1 f2 f20 f3 f4 </dev/null >/dev/null 2>&1 || pc_unresolved "setup: cannot build the archive"
pc_test
ar -dv a f2 >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "ar -dv: exit status $rc"
[ -f out ] || pc_fail "no output file"
printf 'd - f2\n' >exp
cmp -s out exp || pc_fail "ar -dv wrote '$(cat out)', expected 'd - f2'"
[ "$(ar -t a </dev/null | tr '\n' ' ')" = 'f1 f20 f3 f4 ' ] || pc_fail "the archive does not list f1 f20 f3 f4 after the deletion"
ar -p a >o 2>/dev/null </dev/null
[ "$(cat o)" = "$(printf 'one\ntwenty\nthree\nfour')" ] || pc_fail "the remaining members changed"
# Several operands, given in an order that differs from the archive's;
# the order of the output lines is not fixed.
ar -dv a f4 f1 >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "ar -dv a f4 f1: exit status $rc"
[ -f out ] || pc_fail "no output file"
sort out >out.s
printf 'd - f1\nd - f4\n' >exp
cmp -s out.s exp || pc_fail "ar -dv a f4 f1 wrote '$(cat out)'"
[ "$(ar -t a </dev/null | tr '\n' ' ')" = 'f20 f3 ' ] || pc_fail "the archive does not list f20 f3 after the second deletion"
pc_pass

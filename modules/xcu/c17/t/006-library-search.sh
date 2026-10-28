# c17 -L, -l and library operands
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.c17.006
# With two directories each holding a libpcx.a, "-L d1 -L d2 -l pcx" links
# the one in d1 and "-L d2 -L d1" the one in d2; a library given as a file.a
# operand is used as well.
. "$PC_CORE/pc.sh"
pc_begin XCU.c17.006
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
command -v ar >/dev/null 2>&1 || pc_unresolved "setup: ar is not available"
mkdir d1 d2 || pc_unresolved "setup: mkdir failed"
printf 'int pcf(void) { return 1; }\n' >f1.c
printf 'int pcf(void) { return 2; }\n' >f2.c
printf 'int pcf(void);\nint main(void) { return pcf(); }\n' >main.c
c17 -c f1.c </dev/null >/dev/null 2>&1 && c17 -c f2.c </dev/null >/dev/null 2>&1 || pc_unresolved "setup: c17 -c failed"
ar -rc d1/libpcx.a f1.o </dev/null >/dev/null 2>&1 && ar -rc d2/libpcx.a f2.o </dev/null >/dev/null 2>&1 || pc_unresolved "setup: ar failed"
pc_test
c17 -o p1 main.c -L d1 -L d2 -l pcx >out 2>err </dev/null || pc_fail "-L d1 -L d2: nonzero status"
./p1 </dev/null
rc=$?
[ "$rc" -eq 1 ] || pc_fail "-L d1 -L d2 linked the library giving $rc, expected d1's"
c17 -o p2 main.c -L d2 -L d1 -l pcx >out 2>err </dev/null || pc_fail "-L d2 -L d1: nonzero status"
./p2 </dev/null
rc=$?
[ "$rc" -eq 2 ] || pc_fail "-L d2 -L d1 linked the library giving $rc, expected d2's"
c17 -o p3 main.c d2/libpcx.a >out 2>err </dev/null || pc_fail "library operand: nonzero status"
./p3 </dev/null
rc=$?
[ "$rc" -eq 2 ] || pc_fail "the library operand gives $rc, expected 2"
pc_pass

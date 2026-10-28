# c17 -l m and -l c
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.c17.011
# A program that calls sqrt() links with "-l m" and gives the right result;
# "-l c" is accepted.
. "$PC_CORE/pc.sh"
pc_begin XCU.c17.011
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
printf '#include <math.h>\nint main(void) { volatile double x = 16.0; return (int)sqrt(x); }\n' >m.c
printf 'int main(void) { return 6; }\n' >c.c
pc_test
c17 -o pm m.c -l m >out 2>err </dev/null || pc_fail "-l m: nonzero status"
./pm </dev/null
rc=$?
[ "$rc" -eq 4 ] || pc_fail "sqrt(16.0) program exits with $rc, expected 4"
c17 -o pc c.c -l c >out 2>err </dev/null || pc_fail "-l c: nonzero status"
./pc </dev/null
rc=$?
[ "$rc" -eq 6 ] || pc_fail "program linked with -l c exits with $rc, expected 6"
pc_pass

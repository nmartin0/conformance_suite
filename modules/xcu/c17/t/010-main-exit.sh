# return from main is a call to exit
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.c17.010
# Returning a value from main gives that exit status and runs atexit
# handlers; reaching the end of main gives status 0.
. "$PC_CORE/pc.sh"
pc_begin XCU.c17.010
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
printf 'int main(void) { return 3; }\n' >m3.c
printf 'int main(void) { }\n' >m0.c
printf '#include <stdio.h>\n#include <stdlib.h>\nstatic void h(void) { puts("bye"); }\nint main(void) { atexit(h); return 2; }\n' >mh.c
pc_test
c17 -o p3 m3.c >out 2>err </dev/null || pc_fail "m3.c: nonzero status"
./p3 </dev/null
rc=$?
[ "$rc" -eq 3 ] || pc_fail "return 3 gave exit status $rc"
c17 -o p0 m0.c >out 2>err </dev/null || pc_fail "m0.c: nonzero status"
./p0 </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "reaching the end of main gave exit status $rc"
c17 -o ph mh.c >out 2>err </dev/null || pc_fail "mh.c: nonzero status"
./ph >o </dev/null
rc=$?
[ "$rc" -eq 2 ] || pc_fail "return 2 gave exit status $rc"
[ "$(cat o)" = bye ] || pc_fail "the atexit handler did not run on return from main"
pc_pass

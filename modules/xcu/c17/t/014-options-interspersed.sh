# options after operands, main without prototype
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.c17.014
# Options can follow pathname operands, and a definition of main with no or
# two parameters compiles because no prototype is declared for it.
. "$PC_CORE/pc.sh"
pc_begin XCU.c17.014
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
printf '#ifndef PCV\n#define PCV 5\n#endif\nint main(void) { return PCV; }\n' >a.c
printf '#include "h.h"\nint main(void) { return HV; }\n' >b.c
mkdir inc || pc_unresolved "setup: mkdir failed"
printf '#define HV 6\n' >inc/h.h
printf '#include <stdio.h>\n#include <stdlib.h>\n#include <string.h>\n#include <unistd.h>\nint main(int argc, char *argv[]) { return argc > 1 ? 0 : 7; }\n' >c.c
printf '#include <stdio.h>\n#include <stdlib.h>\nint main(void) { return 8; }\n' >d.c
pc_test
c17 a.c -D PCV=3 -o pa </dev/null >out 2>err || pc_fail "c17 a.c -D PCV=3 -o pa failed"
./pa </dev/null
[ "$?" -eq 3 ] || pc_fail "-D after the operand was not applied"
c17 b.c -I inc -o pb </dev/null >out 2>err || pc_fail "c17 b.c -I inc failed"
./pb </dev/null
[ "$?" -eq 6 ] || pc_fail "-I after the operand was not applied"
c17 c.c -o pc </dev/null >out 2>err || pc_fail "a main with two parameters does not compile"
./pc x </dev/null
[ "$?" -eq 0 ] || pc_fail "the two-parameter main did not get its arguments"
c17 d.c -o pd </dev/null >out 2>err || pc_fail "a main with no parameters does not compile"
./pd </dev/null
[ "$?" -eq 8 ] || pc_fail "the no-parameter main gave the wrong status"
pc_pass

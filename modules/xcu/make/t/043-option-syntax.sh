# option syntax
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.make.043
# Options can be grouped, an option-argument can be attached or separate,
# and -- ends the options before the target operands.
. "$PC_CORE/pc.sh"
pc_begin XCU.make.043
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
printf '.POSIX:\nall:\n\techo ran\n' >makefile || pc_unresolved "setup: cannot write the makefile"
pc_test
make -sf makefile all >o 2>err </dev/null || pc_fail "make -sf makefile: nonzero status"
grep -x ran o >/dev/null || pc_fail "-sf makefile did not run the target"
grep 'echo ran' o >/dev/null && pc_fail "-s grouped with -f was not honoured"
make -fmakefile >o 2>err </dev/null || pc_fail "make -fmakefile: nonzero status"
grep -x ran o >/dev/null || pc_fail "-fmakefile did not run the target"
make -j2 >o 2>err </dev/null || pc_fail "make -j2: nonzero status"
grep -x ran o >/dev/null || pc_fail "-j2 did not run the target"
make -ik -- all >o 2>err </dev/null || pc_fail "make -ik -- all: nonzero status"
grep -x ran o >/dev/null || pc_fail "-- before the operand did not run the target"
pc_pass

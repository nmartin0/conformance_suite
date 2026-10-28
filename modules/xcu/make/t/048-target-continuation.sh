# backslash-newline in a target line
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.make.048
# A backslash-newline in a target line joins the prerequisites of the next
# line.
. "$PC_CORE/pc.sh"
pc_begin XCU.make.048
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
printf '.POSIX:\nall: a \\\n    b\na:\n\t@echo a-ran\nb:\n\t@echo b-ran\n' >makefile || pc_unresolved "setup: cannot write the makefile"
pc_test
make >o 2>err </dev/null || pc_fail "make: nonzero status"
grep -x a-ran o >/dev/null || pc_fail "a was not made"
grep -x b-ran o >/dev/null || pc_fail "b on the continuation line was not made"
pc_pass

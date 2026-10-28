# built-in rules
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.make.038
# The built-in .sh rule copies the script and makes the copy executable, and
# a rule .c.o in a makefile replaces the built-in one.
. "$PC_CORE/pc.sh"
pc_begin XCU.make.038
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
printf 'echo ran-tool\n' >tool.sh || pc_unresolved "setup: cannot write tool.sh"
printf 'csrc\n' >x.c || pc_unresolved "setup: cannot write x.c"
printf '.POSIX:\n' >plain.mk
printf '.POSIX:\n.c.o:\n\tcp $< $@\n' >redef.mk
pc_test
make -f plain.mk tool >o 2>err </dev/null || pc_fail "make tool: nonzero status"
[ -x tool ] || pc_fail "the .sh rule did not make an executable file"
cmp tool tool.sh >/dev/null 2>&1 || pc_fail "the .sh rule did not copy the script"
sh ./tool >o 2>err </dev/null
grep -x ran-tool o >/dev/null || pc_fail "the copy of the script does not run"
make -f redef.mk x.o >o 2>err </dev/null || pc_fail "make x.o: nonzero status"
cmp x.o x.c >/dev/null 2>&1 || pc_fail "the .c.o rule of the makefile was not used"
pc_pass

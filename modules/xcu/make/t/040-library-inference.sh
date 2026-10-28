# internal macros in a .s2.a rule
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.make.040
# For lib.a(m.o) built by a .x.a rule, $< is m.x, $* is m, $@ is lib.a,
# $^ and $? are m.x and $% is m.o.
. "$PC_CORE/pc.sh"
pc_begin XCU.make.040
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
printf '.POSIX:\n.SUFFIXES: .x .a\n.x.a:\n\t@echo "<$<> <$*> <$@> <$^> <$?> <$%%>"\n' >makefile
: >m.x || pc_unresolved "setup: cannot make m.x"
pc_test
make 'lib.a(m.o)' >o 2>err </dev/null || pc_fail "make lib.a(m.o): nonzero status: $(cat err)"
grep -Fx '<m.x> <m> <lib.a> <m.x> <m.x> <m.o>' o >/dev/null || pc_fail "internal macros in the .x.a rule: $(cat o)"
pc_pass

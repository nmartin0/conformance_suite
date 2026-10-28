# an empty inference rule
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.make.039
# A rule with only a semicolon is an empty inference rule: it is found and
# run without effect, so the target is not built and there is no error.
. "$PC_CORE/pc.sh"
pc_begin XCU.make.039
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
printf 'this is not C\n' >x.c || pc_unresolved "setup: cannot write x.c"
printf '.POSIX:\n.SUFFIXES: .c .o\n.c.o: ;\n' >e1.mk
printf '.POSIX:\n.SUFFIXES: .c .o\n.c.o :   ;\n' >e2.mk
pc_test
for f in e1.mk e2.mk; do
	make -f $f x.o >o 2>err </dev/null || pc_fail "$f: make x.o failed: $(cat err)"
	[ -e x.o ] && pc_fail "$f: the empty rule produced x.o"
done
pc_pass

# forms of the include line
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.make.045
# An include line can name several files and end in a comment, and a
# relative name is taken from the working directory, not from the directory
# of the makefile.
. "$PC_CORE/pc.sh"
pc_begin XCU.make.045
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
printf 'PCA = 1\n' >a.mk && printf 'PCB = 2\n' >b.mk || pc_unresolved "setup: cannot write the include files"
printf '.POSIX:\ninclude a.mk b.mk # two files and a comment\nall:\n\t@echo "$(PCA)$(PCB)"\n' >makefile
mkdir sub || pc_unresolved "setup: cannot make a directory"
printf 'PCC = right\n' >inc.mk
printf 'PCC = wrong\n' >sub/inc.mk
printf '.POSIX:\ninclude inc.mk\nall:\n\t@echo "$(PCC)"\n' >sub/mk
pc_test
make >o 2>err </dev/null || pc_fail "make: nonzero status"
grep -Fx 12 o >/dev/null || pc_fail "an include line with two files and a comment: $(cat o)"
make -f sub/mk >o 2>err </dev/null || pc_fail "make -f sub/mk: nonzero status"
grep -Fx right o >/dev/null || pc_fail "an included name was not taken relative to the working directory: $(cat o)"
pc_pass

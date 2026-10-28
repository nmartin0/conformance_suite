# escaped newlines
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.make.017
# A backslash-newline in a macro definition joins the lines, and one in a
# command line is passed to the shell with the next line (minus a leading
# tab).
. "$PC_CORE/pc.sh"
pc_begin XCU.make.017
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
printf '.POSIX:\nLONG = one\\\n   two\nall:\n	echo "[$(LONG)]"\n	echo a \\\n	b\n' >makefile
pc_test
make -s >o 2>err </dev/null || pc_fail "make: nonzero status"
printf '[one two]\na b\n' >exp
cmp -s o exp || pc_fail "continuation lines gave: $(cat o)"
pc_pass

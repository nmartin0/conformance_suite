# macro expansion forms
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.make.012
# $(X:.c=.o) and ${X:.c=.o} replace a suffix of each word, $$ gives a dollar
# sign for the shell, a one-character macro needs no parentheses, and an
# undefined macro gives an empty string.
. "$PC_CORE/pc.sh"
pc_begin XCU.make.012
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
cat >makefile <<'EOM'
.POSIX:
SRC = a.c b.c dir/c.c
OBJ = $(SRC:.c=.o)
BR = ${SRC:.c=.o}
X = x
all:
	echo "OBJ=$(OBJ)"
	echo "BR=$(BR)"
	echo "ONE=[$X]"
	echo "UNDEF=[$(NOSUCHMACRO)]"
	V=shellval; echo "SH=$$V"
EOM
cat >exp <<'EOM'
OBJ=a.o b.o dir/c.o
BR=a.o b.o dir/c.o
ONE=[x]
UNDEF=[]
SH=shellval
EOM
pc_test
make -s >o 2>err </dev/null || pc_fail "make: nonzero status"
cmp -s o exp || pc_fail "expansions differ from the expected text: $(cat o)"
pc_pass

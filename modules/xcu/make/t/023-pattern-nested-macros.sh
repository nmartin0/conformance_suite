# pattern macro expansions and nested references
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.make.023
# $(X:%.c=%.x) and $(X:a.%=new_%) replace by pattern, and nested references
# like $($(N)) are expanded inside out.
. "$PC_CORE/pc.sh"
pc_begin XCU.make.023
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
cat >makefile <<'EOM'
.POSIX:
SRC = a.c b.c dir/c.c
PAT = $(SRC:%.c=%.x)
PRE = $(SRC:a.%=new_%)
N = one
one = single
all:
	echo "PAT=$(PAT)"
	echo "PRE=$(PRE)"
	echo "NEST=$($(N))"
EOM
cat >exp <<'EOM'
PAT=a.x b.x dir/c.x
PRE=new_c b.c dir/c.c
NEST=single
EOM
pc_test
make -s >o 2>err </dev/null || pc_fail "make: nonzero status"
cmp -s o exp || pc_fail "expansions differ from the expected text: $(cat o)"
pc_pass

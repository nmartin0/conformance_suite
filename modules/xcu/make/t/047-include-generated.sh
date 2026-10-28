# an include file made by a rule
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.make.047
# An include file that does not exist is made with the rule for it and then
# read, also with -n, which still runs the commands that make it.
. "$PC_CORE/pc.sh"
pc_begin XCU.make.047
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
cat >makefile <<'EOM'
.POSIX:
gen.mk:
	echo 'PCG = generated' >gen.mk
include gen.mk
all:
	@echo $(PCG)
EOM
pc_test
make all >o 2>err </dev/null || pc_fail "make all: nonzero status: $(cat err)"
grep -x generated o >/dev/null || pc_fail "the generated include file was not made and used: $(cat o)"
rm -f gen.mk
make -n all >o 2>err </dev/null || pc_fail "make -n: nonzero status"
[ -f gen.mk ] || pc_fail "make -n did not run the command that makes the include file"
pc_pass

# macros reach the environment, options reach a sub-make
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.make.029
# Macros given on the command line are added to the environment of the
# commands, macros defined only in the makefile are not, and the options
# and macros of the command line are passed on to a sub-make.
. "$PC_CORE/pc.sh"
pc_begin XCU.make.029
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
unset PCX PCY PCQ
cat >makefile <<'EOM'
.POSIX:
PCY = fromfile
all:
	@echo "x=[$$PCX] y=[$$PCY]"
EOM
cat >top.mk <<'EOM'
.POSIX:
all:
	$(MAKE) -f sub.mk
EOM
cat >sub.mk <<'EOM'
.POSIX:
all: bad good
bad:
	false
good:
	echo sub-good $(PCQ)
EOM
pc_test
make PCX=cmdval >o 2>err </dev/null || pc_fail "make PCX=cmdval: nonzero status"
grep -Fx 'x=[cmdval] y=[]' o >/dev/null || pc_fail "environment of the command: $(cat o)"
make -k -f top.mk PCQ='a b c' >o 2>err </dev/null
grep -Fx 'sub-good a b c' o >/dev/null || pc_fail "the sub-make did not get -k and the command line macro: $(cat o)"
make -f top.mk >o 2>err </dev/null
grep -x 'sub-good' o >/dev/null && pc_fail "without -k the sub-make went on after a failure"
pc_pass

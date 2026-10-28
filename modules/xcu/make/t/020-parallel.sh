# make -j
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.make.020
# "make -j 2" with four independent targets builds them all and exits 0, and
# a .WAIT prerequisite makes the targets to its right start after those to
# its left are done.
. "$PC_CORE/pc.sh"
pc_begin XCU.make.020
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
cat >makefile <<'EOM'
.POSIX:
all: a b c d
a b c d:
	touch $@
EOM
cat >wait.mk <<'EOM'
.POSIX:
all: x .WAIT y
x:
	touch x.done
y:
	test -f x.done
EOM
pc_test
make -j 2 >o 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "make -j 2: exit status $rc"
[ -f a ] && [ -f b ] && [ -f c ] && [ -f d ] || pc_fail "-j 2 did not build every target"
make -j 3 -f wait.mk >o 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail ".WAIT: exit status $rc (y ran before x finished?)"
pc_pass

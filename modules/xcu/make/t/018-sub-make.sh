# $(MAKE), -n propagation and MAKEFLAGS
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.make.018
# Under -n a command line using $(MAKE) is still run, and the sub-make also
# behaves as if -n, so the sub-make's own command is written and not run;
# MAKEFLAGS=n and MAKEFLAGS=-n in the environment act as -n.
. "$PC_CORE/pc.sh"
pc_begin XCU.make.018
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
cat >makefile <<'EOM'
.POSIX:
all:
	$(MAKE) -f sub.mk t
EOM
cat >sub.mk <<'EOM'
.POSIX:
t:
	touch subran
EOM
cat >plain.mk <<'EOM'
.POSIX:
t:
	touch envran
EOM
pc_test
make -n >o 2>err </dev/null || pc_fail "make -n: nonzero status"
[ -e subran ] && pc_fail "the sub-make ran its command under -n"
sed 's/^[[:blank:]]*//' o | grep -x 'touch subran' >/dev/null || pc_fail "the sub-make did not run (its command was not written)"
make -s >o 2>err </dev/null || pc_fail "make: nonzero status"
[ -e subran ] || pc_fail "the sub-make did not run its command normally"
MAKEFLAGS=n make -f plain.mk t >o 2>err </dev/null || pc_fail "MAKEFLAGS=n: nonzero status"
[ -e envran ] && pc_fail "MAKEFLAGS=n did not act as -n"
MAKEFLAGS=-n make -f plain.mk t >o 2>err </dev/null || pc_fail "MAKEFLAGS=-n: nonzero status"
[ -e envran ] && pc_fail "MAKEFLAGS=-n did not act as -n"
pc_pass

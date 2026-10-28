# CURDIR and SHELL
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.make.019
# $(CURDIR) is the working directory (the same as pwd -L or pwd -P); the
# SHELL environment variable does not change the SHELL macro or stop
# commands running.
. "$PC_CORE/pc.sh"
pc_begin XCU.make.019
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
cat >makefile <<'EOM'
.POSIX:
all:
	echo "$(CURDIR)"
	echo "[$(SHELL)]"
EOM
cat >lp <<'EOM'
EOM
pc_test
make -s >o 2>err </dev/null || pc_fail "make: nonzero status"
cd=$(sed -n 1p o)
l=$(pwd -L)
p=$(pwd -P)
{ [ "$cd" = "$l" ] || [ "$cd" = "$p" ]; } || pc_fail "CURDIR is '$cd', expected '$l' or '$p'"
SHELL=/nonexistent/shell make -s >o 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "with SHELL set in the environment make failed: exit status $rc"
[ "$(sed -n 2p o)" = '[/nonexistent/shell]' ] && pc_fail "the SHELL environment variable changed the SHELL macro"
pc_pass

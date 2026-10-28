# -t and phony targets
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.make.051
# With -t a phony target is not touched and its commands are not run, while
# an ordinary target with commands is touched; a command with a + prefix
# is still run.
. "$PC_CORE/pc.sh"
pc_begin XCU.make.051
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
cat >makefile <<'EOM'
.POSIX:
.PHONY: p
p:
	@echo p-ran
q:
	@echo q-ran
r:
	+@echo r-ran
EOM
pc_test
make -t p q r >o 2>err </dev/null || pc_fail "make -t: nonzero status"
[ -e p ] && pc_fail "-t touched the phony target"
[ -e q ] || pc_fail "-t did not touch the ordinary target"
grep -e p-ran -e q-ran o >/dev/null && pc_fail "-t ran a command without a + prefix"
grep -x r-ran o >/dev/null || pc_fail "-t did not run the command with a + prefix"
pc_pass

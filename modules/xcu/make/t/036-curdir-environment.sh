# CURDIR and the environment
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.make.036
# A CURDIR variable in the environment does not change the CURDIR macro
# unless -e is given and the environment then holds the macro value; CURDIR
# given on the command line replaces the macro without changing directory.
. "$PC_CORE/pc.sh"
pc_begin XCU.make.036
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
cat >makefile <<'EOM'
.POSIX:
all:
	@echo "macro=$(CURDIR) env=$$CURDIR"
	@pwd
EOM
L=$(pwd -L) && P=$(pwd -P) || pc_unresolved "setup: pwd failed"
pc_test
CURDIR=/nonexistent-pcdir make >o 2>err </dev/null || pc_fail "make: nonzero status"
ok=0
for d in "$L" "$P"; do
	grep -Fx "macro=$d env=$d" o >/dev/null && ok=1
done
[ "$ok" -eq 1 ] || pc_fail "the environment CURDIR changed the macro or was not reset: $(cat o)"
CURDIR=/nonexistent-pcdir make -e >o 2>err </dev/null || pc_fail "make -e: nonzero status"
grep -F 'macro=/nonexistent-pcdir' o >/dev/null || pc_fail "-e did not let the environment set CURDIR: $(cat o)"
make CURDIR=/elsewhere-pcdir >o 2>err </dev/null || pc_fail "make CURDIR=...: nonzero status"
grep -F 'macro=/elsewhere-pcdir' o >/dev/null || pc_fail "CURDIR on the command line did not replace the macro: $(cat o)"
okd=0
for d in "$L" "$P"; do grep -Fx "$d" o >/dev/null && okd=1; done
[ "$okd" -eq 1 ] || pc_fail "defining CURDIR changed the directory of the commands: $(cat o)"
pc_pass

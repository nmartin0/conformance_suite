# the SHELL macro and the SHELL variable
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.make.037
# Defining SHELL in the makefile or on the command line does not change the
# SHELL environment variable seen by the commands.
. "$PC_CORE/pc.sh"
pc_begin XCU.make.037
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
shpath=$(command -v sh) || pc_unresolved "setup: no sh found"
case $shpath in /*) ;; *) pc_unresolved "setup: no absolute path for sh" ;; esac
cat >makefile <<EOM
.POSIX:
SHELL = $shpath
all:
	@echo "env=[\$\$SHELL] mac=[\$(SHELL)]"
EOM
cat >plain.mk <<'EOM'
.POSIX:
all:
	@echo "env=[$$SHELL] mac=[$(SHELL)]"
EOM
pc_test
SHELL=pc-env-shell-value; export SHELL
make >o 2>err </dev/null || pc_fail "make: nonzero status"
grep -Fx "env=[pc-env-shell-value] mac=[$shpath]" o >/dev/null || pc_fail "SHELL in the makefile: $(cat o)"
make -f plain.mk SHELL="$shpath" >o 2>err </dev/null || pc_fail "make SHELL=...: nonzero status"
grep -Fx "env=[pc-env-shell-value] mac=[$shpath]" o >/dev/null || pc_fail "SHELL on the command line: $(cat o)"
pc_pass

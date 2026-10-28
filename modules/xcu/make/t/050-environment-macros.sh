# environment variables as macros
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.make.050
# Variables of the environment, also those with a null value, are macros and
# are in the environment of the commands.
. "$PC_CORE/pc.sh"
pc_begin XCU.make.050
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
cat >makefile <<'EOM'
.POSIX:
PCNULL ?= dflt
all:
	@echo "[$$PCENV] [$(PCENV)] [$(PCNULL)] [$$PCNULL]"
EOM
pc_test
PCENV=envval PCNULL= make >o 2>err </dev/null || pc_fail "make: nonzero status"
grep -Fx '[envval] [envval] [] []' o >/dev/null || pc_fail "environment macros: $(cat o)"
pc_pass

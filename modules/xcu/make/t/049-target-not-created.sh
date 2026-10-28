# a target that its commands did not create
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.make.049
# When a target does not exist after its commands succeeded it counts as
# newer than the targets that need it, which are then made too.
. "$PC_CORE/pc.sh"
pc_begin XCU.make.049
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
cat >makefile <<'EOM'
.POSIX:
all: t
	@echo all-ran
t:
	@echo t-ran
EOM
: >all || pc_unresolved "setup: cannot make the file all"
pc_test
make all >o 2>err </dev/null || pc_fail "make all: nonzero status"
grep -x t-ran o >/dev/null || pc_fail "t was not made"
grep -x all-ran o >/dev/null || pc_fail "all was not remade after its prerequisite was not created"
pc_pass

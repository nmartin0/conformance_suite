# several targets, command after a semicolon, empty rule
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.make.033
# A rule with several targets gives each the commands (a command after a
# semicolon is the first one, tab lines add to it), and a rule with only a
# semicolon is an empty rule that does nothing.
. "$PC_CORE/pc.sh"
pc_begin XCU.make.033
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
cat >makefile <<'EOM'
.POSIX:
t1 t2: ; @echo "built $@"
t3: ; @echo one
	@echo two
e: ;
all: t1 t2 t3 e
EOM
pc_test
make all >o 2>err </dev/null || pc_fail "make all: nonzero status"
grep -Fx 'built t1' o >/dev/null || pc_fail "t1 did not get the commands of the shared rule"
grep -Fx 'built t2' o >/dev/null || pc_fail "t2 did not get the commands of the shared rule"
l1=$(grep -n -x one o | cut -d: -f1); l2=$(grep -n -x two o | cut -d: -f1)
{ [ -n "$l1" ] && [ -n "$l2" ] && [ "$l1" -lt "$l2" ]; } || pc_fail "one and two are not both written in order"
make e >o 2>err </dev/null || pc_fail "make e: the empty rule failed"
pc_pass

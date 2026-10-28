# command echo, @ prefix, -s and .SILENT
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.make.004
# Commands are written to standard output before execution except those with
# a @ prefix; with -s, or with .SILENT without prerequisites, or with the
# target listed after .SILENT, none is written; the commands' own output is
# unaffected.
. "$PC_CORE/pc.sh"
pc_begin XCU.make.004
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
cat >makefile <<'EOM'
.POSIX:
t:
	echo visible
	@echo hidden
EOM
cat >sil1.mk <<'EOM'
.POSIX:
.SILENT:
t:
	echo visible
	@echo hidden
EOM
cat >sil2.mk <<'EOM'
.POSIX:
.SILENT: t
t:
	echo visible
	@echo hidden
EOM
printf 'visible\nhidden\n' >exp
pc_test
make t >o 2>err </dev/null || pc_fail "make t: nonzero status"
sed 's/^[[:blank:]]*//' o >o.s
grep -x 'echo visible' o.s >/dev/null || pc_fail "the command was not written before running"
grep -x 'echo hidden' o.s >/dev/null && pc_fail "an @ command was written"
grep -x 'visible' o.s >/dev/null || pc_fail "the output of the first command is missing"
grep -x 'hidden' o.s >/dev/null || pc_fail "the output of the @ command is missing"
make -s t >o 2>err </dev/null || pc_fail "make -s t: nonzero status"
cmp -s o exp || pc_fail "-s still wrote commands: $(cat o)"
make -f sil1.mk t >o 2>err </dev/null || pc_fail ".SILENT: nonzero status"
cmp -s o exp || pc_fail ".SILENT without prerequisites still wrote commands: $(cat o)"
make -f sil2.mk t >o 2>err </dev/null || pc_fail ".SILENT: t: nonzero status"
cmp -s o exp || pc_fail ".SILENT with the target still wrote commands: $(cat o)"
pc_pass

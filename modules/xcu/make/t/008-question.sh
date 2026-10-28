# make -q
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.make.008
# "make -q" exits 0 when the target is up to date and 1 when it is not, and
# does not run its commands, except that a + prefixed command is run.
. "$PC_CORE/pc.sh"
pc_begin XCU.make.008
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
cat >makefile <<'EOM'
.POSIX:
t: src
	touch t-ran
p: src
	+touch plus-ran
EOM
: >src; : >t; : >p
touch -t 200001010000 src || pc_unresolved "setup: touch failed"
pc_test
make -q t >o 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "-q on an up-to-date target: exit status $rc"
touch -t 200101010000 t p
touch -t 200301010000 src
make -q t >o 2>err </dev/null
rc=$?
[ "$rc" -eq 1 ] || pc_fail "-q on an out-of-date target: exit status $rc, expected 1"
[ -e t-ran ] && pc_fail "-q ran the commands"
make -q p >o 2>err </dev/null
rc=$?
[ "$rc" -eq 1 ] || pc_fail "-q on an out-of-date target with a + command: exit status $rc, expected 1"
[ -e plus-ran ] || pc_fail "-q did not run the + command"
pc_pass

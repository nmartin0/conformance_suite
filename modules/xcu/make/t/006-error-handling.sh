# command failure, -, -i and .IGNORE
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.make.006
# A failing command stops make with nonzero status before later commands; a
# - prefix, -i, or a .IGNORE special target make the failure ignored (status
# 0, later commands run); a command line is run with the shell -e option
# unless errors are ignored.
. "$PC_CORE/pc.sh"
pc_begin XCU.make.006
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
cat >makefile <<'EOM'
.POSIX:
fail:
	false
	echo after-fail
ign:
	-false
	echo after-ign
sh:
	false; echo same-line
EOM
cat >ignore.mk <<'EOM'
.POSIX:
.IGNORE:
fail:
	false
	echo after-fail
EOM
pc_test
make fail >o 2>err </dev/null
rc=$?
[ "$rc" -ne 0 ] || pc_fail "a failing command gave status 0"
grep -x 'after-fail' o >/dev/null && pc_fail "make went on after a failing command"
make ign >o 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "a - prefixed failure gave status $rc"
grep -x 'after-ign' o >/dev/null || pc_fail "make stopped after a - prefixed failure"
make -i fail >o 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "-i: exit status $rc"
grep -x 'after-fail' o >/dev/null || pc_fail "-i did not go on after the failure"
make -f ignore.mk fail >o 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail ".IGNORE: exit status $rc"
grep -x 'after-fail' o >/dev/null || pc_fail ".IGNORE did not go on after the failure"
make sh >o 2>err </dev/null
rc=$?
[ "$rc" -ne 0 ] || pc_fail "a command line failing at its first command gave status 0"
grep -x 'same-line' o >/dev/null && pc_fail "the shell was run without -e"
pc_pass

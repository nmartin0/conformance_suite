# existing files, .DEFAULT and the default target
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.make.015
# An existing file with no prerequisites is up to date and its commands do
# not run; .DEFAULT commands build any target with no rule, with $< the
# target name; the first target after special ones is the default.
. "$PC_CORE/pc.sh"
pc_begin XCU.make.015
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
cat >plain.mk <<'EOM'
.POSIX:
clean:
	echo cleaning
EOM
cat >default.mk <<'EOM'
.POSIX:
.DEFAULT:
	echo "default $<"
EOM
cat >first.mk <<'EOM'
.POSIX:
.SILENT:
a:
	echo run-a
b:
	echo run-b
EOM
: >clean
pc_test
make -s -f plain.mk clean >o 2>err </dev/null || pc_fail "plain target: nonzero status"
grep -x cleaning o >/dev/null && pc_fail "an existing file without prerequisites was rebuilt"
make -s -f default.mk unknown.thing >o 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail ".DEFAULT: exit status $rc"
grep -x 'default unknown.thing' o >/dev/null || pc_fail ".DEFAULT commands did not run with \$< set to the target"
make -s -f first.mk >o 2>err </dev/null || pc_fail "first.mk: nonzero status"
grep -x run-a o >/dev/null || pc_fail "the first non-special target was not the default"
pc_pass

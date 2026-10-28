# default target, operand order, missing rule
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.make.002
# With no operand make builds the first target that is not special; "make b
# a" builds b then a; a target with no rule that does not exist is an error
# (nonzero status); an existing file with no rule is up to date (status 0).
. "$PC_CORE/pc.sh"
pc_begin XCU.make.002
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
cat >makefile <<'EOM'
.POSIX:
a:
	echo build-a
b:
	echo build-b
EOM
: >exist.file || pc_unresolved "setup: cannot create a file"
pc_test
make >o 2>err </dev/null || pc_fail "make: nonzero status"
grep -x 'build-a' o >/dev/null || pc_fail "the first target was not built by default"
grep -x 'build-b' o >/dev/null && pc_fail "the second target was built by default"
make b a >o 2>err </dev/null || pc_fail "make b a: nonzero status"
got=$(grep -x 'build-.' o | tr '\n' ' ')
[ "$got" = 'build-b build-a ' ] || pc_fail "targets were built in order '$got'"
make nosuch.target >o 2>err </dev/null
rc=$?
[ "$rc" -ne 0 ] || pc_fail "a target with no rule that does not exist gave status 0"
make exist.file >o 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "an existing file with no rule gave status $rc"
pc_pass

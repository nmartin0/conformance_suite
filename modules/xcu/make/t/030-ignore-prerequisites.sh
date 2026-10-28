# .IGNORE with prerequisites
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.make.030
# .IGNORE with prerequisites ignores errors only for the targets it names,
# and later .IGNORE lines add to the list.
. "$PC_CORE/pc.sh"
pc_begin XCU.make.030
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
cat >makefile <<'EOM'
.POSIX:
.IGNORE: a
a:
	false
	echo a-continued
b:
	false
	echo b-continued
EOM
cat >two.mk <<'EOM'
.POSIX:
.IGNORE: a
.IGNORE: b
a:
	false
	echo a-continued
b:
	false
	echo b-continued
EOM
pc_test
make a >o 2>err </dev/null || pc_fail "make a: nonzero status although a is ignored"
grep -x a-continued o >/dev/null || pc_fail "the command after the ignored failure did not run"
make b >o 2>err </dev/null && pc_fail "make b: status 0 although b is not ignored"
grep -x b-continued o >/dev/null && pc_fail "b went on after an error"
make -f two.mk b >o 2>err </dev/null || pc_fail "two .IGNORE lines: b was not ignored"
grep -x b-continued o >/dev/null || pc_fail "two .IGNORE lines: b did not go on"
pc_pass

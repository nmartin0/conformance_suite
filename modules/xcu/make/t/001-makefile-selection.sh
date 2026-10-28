# make chooses the makefile
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.make.001
# With both ./makefile and ./Makefile present, ./makefile is used; with only
# ./Makefile it is used; -f NAME uses that file instead; -f - reads the
# makefile from standard input; with several -f options the first target
# found in the first file is the default.
. "$PC_CORE/pc.sh"
pc_begin XCU.make.001
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
cat >makefile <<'EOM'
.POSIX:
all:
	echo from-makefile
EOM
cat >Makefile <<'EOM'
.POSIX:
all:
	echo from-Makefile
EOM
cat >other.mk <<'EOM'
.POSIX:
all:
	echo from-other
EOM
cat >a.mk <<'EOM'
first:
	echo run-first
EOM
cat >b.mk <<'EOM'
second:
	echo run-second
EOM
[ -f makefile ] && [ -f Makefile ] || pc_unresolved "setup: cannot write the makefiles"
pc_test
make >o 2>err </dev/null || pc_fail "make with both default files: nonzero status"
grep -x 'from-makefile' o >/dev/null || pc_fail "./makefile was not used"
grep -x 'from-Makefile' o >/dev/null && pc_fail "./Makefile was used although ./makefile exists"
make -f other.mk >o 2>err </dev/null || pc_fail "make -f: nonzero status"
grep -x 'from-other' o >/dev/null || pc_fail "-f did not select the named file"
grep -x 'from-makefile' o >/dev/null && pc_fail "-f still ran the default file"
make -f - >o 2>err <other.mk || pc_fail "make -f -: nonzero status"
grep -x 'from-other' o >/dev/null || pc_fail "-f - did not read the standard input"
make -f a.mk -f b.mk >o 2>err </dev/null || pc_fail "make -f a.mk -f b.mk: nonzero status"
grep -x 'run-first' o >/dev/null || pc_fail "the first target of the first -f file was not the default"
grep -x 'run-second' o >/dev/null && pc_fail "the second -f file supplied the default target"
make -f b.mk -f a.mk >o 2>err </dev/null || pc_fail "make -f b.mk -f a.mk: nonzero status"
grep -x 'run-second' o >/dev/null || pc_fail "the files were not processed in the order given"
rm -f makefile
make >o 2>err </dev/null || pc_fail "make with only ./Makefile: nonzero status"
grep -x 'from-Makefile' o >/dev/null || pc_fail "./Makefile was not used when it is the only default file"
pc_pass

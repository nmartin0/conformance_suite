# MAKEFLAGS option formats and order
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.make.028
# MAKEFLAGS can hold options with or without leading hyphens; options in
# it are taken before those on the command line, and when both -k and -S are
# given the last one wins.
. "$PC_CORE/pc.sh"
pc_begin XCU.make.028
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
cat >makefile <<'EOM'
.POSIX:
all: bad good
bad:
	false
good:
	echo good-built
EOM
pc_test
MAKEFLAGS=k make >o 2>err </dev/null; st=$?
[ "$st" -ne 0 ] || pc_fail "MAKEFLAGS=k: status 0 although a target failed"
grep -x good-built o >/dev/null || pc_fail "MAKEFLAGS=k did not act as -k"
MAKEFLAGS=-k make >o 2>err </dev/null
grep -x good-built o >/dev/null || pc_fail "MAKEFLAGS=-k did not act as -k"
MAKEFLAGS=-k make -S >o 2>err </dev/null; st=$?
[ "$st" -ne 0 ] || pc_fail "MAKEFLAGS=-k with -S: status 0"
grep -x good-built o >/dev/null && pc_fail "-S on the command line did not override -k from MAKEFLAGS"
MAKEFLAGS=-S make -k >o 2>err </dev/null
grep -x good-built o >/dev/null || pc_fail "-k on the command line did not override -S from MAKEFLAGS"
printf '.POSIX:\nall:\n\techo "[$(PCV)]"\n' >val.mk
MAKEFLAGS='-s PCV=flagval' make -f val.mk >o 2>err </dev/null || pc_fail "MAKEFLAGS with a macro: nonzero status"
grep -Fx '[flagval]' o >/dev/null || pc_fail "a macro definition in MAKEFLAGS was not used: $(cat o)"
grep -F 'echo' o >/dev/null && pc_fail "-s in MAKEFLAGS (with a macro) did not silence the command"
pc_pass

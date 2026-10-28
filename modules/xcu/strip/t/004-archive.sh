# strip on an archive of object files
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.strip.004
# "strip lib.a" for an archive made by ar -r from an object file exits 0 and
# the archive still lists its member.
. "$PC_CORE/pc.sh"
pc_begin XCU.strip.004
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
command -v c17 >/dev/null 2>&1 || pc_unresolved "setup: c17 is not available"
printf 'int pcsym(void) { return 1; }\n' >x.c
c17 -c x.c </dev/null >/dev/null 2>&1 || pc_unresolved "setup: c17 -c failed"
ar -rc lib.a x.o </dev/null >/dev/null 2>&1 || pc_unresolved "setup: ar failed"
pc_test
strip lib.a >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "strip of an archive: exit status $rc"
[ "$(ar -t lib.a </dev/null)" = x.o ] || pc_fail "the stripped archive no longer lists x.o"
pc_pass

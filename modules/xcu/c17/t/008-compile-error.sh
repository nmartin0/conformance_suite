# c17 error handling
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.c17.008
# "c17 -c bad.c good.c" with a source error in bad.c exits with a nonzero
# status, writes a diagnostic to standard error, creates no bad.o, and still
# compiles good.c to good.o.
. "$PC_CORE/pc.sh"
pc_begin XCU.c17.008
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
printf 'int main(void) { this is not C; }\n' >bad.c
printf 'int pcg(void) { return 1; }\n' >good.c
pc_test
c17 -c bad.c good.c >out 2>err </dev/null
rc=$?
[ "$rc" -ne 0 ] || pc_fail "a source error gave exit status 0"
[ -s err ] || pc_fail "no diagnostic on standard error"
[ -e bad.o ] && pc_fail "an object file was made for the bad source"
[ -f good.o ] || pc_fail "the good source was not compiled after the error"
pc_pass

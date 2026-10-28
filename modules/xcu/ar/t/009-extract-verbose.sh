# ar -xv output
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ar.009
# "ar -xv a f1" writes "x - f1" to standard output.
. "$PC_CORE/pc.sh"
pc_begin XCU.ar.009
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
printf 'one\n' >f1; printf 'two\n' >f2
ar -rc a f1 f2 </dev/null >/dev/null 2>&1 || pc_unresolved "setup: cannot build the archive"
rm -f f1 f2
pc_test
ar -xv a f1 >out 2>err </dev/null || pc_fail "ar -xv a f1: nonzero status"
[ "$(cat out)" = 'x - f1' ] || pc_fail "ar -xv a f1 wrote '$(cat out)'"
ar -xv a >out 2>err </dev/null || pc_fail "ar -xv a: nonzero status"
[ "$(cat out)" = "$(printf 'x - f1\nx - f2')" ] || pc_fail "ar -xv a wrote '$(cat out)'"
pc_pass

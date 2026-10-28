# ar -t lists member names
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ar.002
# "ar -t a" prints the member names one per line in archive order; with one
# operand only that member is listed, printed as the operand was given.
. "$PC_CORE/pc.sh"
pc_begin XCU.ar.002
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
printf 'one\n' >f1; printf 'two\n' >f2; printf 'three\n' >f3
ar -rc a f1 f2 f3 </dev/null >/dev/null 2>&1 || pc_unresolved "setup: cannot build the archive"
exp=$(printf 'f1\nf2\nf3')
pc_test
got=$(ar -t a </dev/null)
[ "$got" = "$exp" ] || pc_fail "ar -t listed '$got'"
got=$(ar -t a f2 </dev/null)
[ "$got" = f2 ] || pc_fail "ar -t a f2 listed '$got'"
got=$(ar -t a sub/f2 </dev/null)
[ "$got" = sub/f2 ] || pc_fail "ar -t a sub/f2 listed '$got', expected the operand as given"
pc_pass

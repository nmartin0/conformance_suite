# ar -d deletes members
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ar.005
# "ar -d a f2" removes f2 and keeps the others in their order; with -v it
# writes "d - f2" to standard output.
. "$PC_CORE/pc.sh"
pc_begin XCU.ar.005
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
printf 'one\n' >f1; printf 'two\n' >f2; printf 'three\n' >f3
ar -rc a f1 f2 f3 </dev/null >/dev/null 2>&1 || pc_unresolved "setup: cannot build the archive"
pc_test
ar -dv a f2 >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "ar -dv: exit status $rc"
[ "$(cat out)" = 'd - f2' ] || pc_fail "ar -dv wrote '$(cat out)', expected 'd - f2'"
[ "$(ar -t a </dev/null)" = "$(printf 'f1\nf3')" ] || pc_fail "the archive does not list f1 and f3 after the deletion"
pc_pass

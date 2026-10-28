# ar -r replaces in place and appends new files
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ar.006
# Replacing an existing member keeps its place in the archive and takes the
# new contents; a file not yet in the archive is appended at the end; with
# -v standard output has "r - NAME" for a replaced file and "a - NAME" for
# an added one.
. "$PC_CORE/pc.sh"
pc_begin XCU.ar.006
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
printf 'one\n' >f1; printf 'two\n' >f2; printf 'three\n' >f3
ar -rc a f1 f2 f3 </dev/null >/dev/null 2>&1 || pc_unresolved "setup: cannot build the archive"
printf 'two-new\n' >f2; printf 'four\n' >f4
pc_test
ar -rv a f2 f4 >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "ar -rv: exit status $rc"
[ "$(cat out)" = "$(printf 'r - f2\na - f4')" ] || pc_fail "ar -rv wrote '$(cat out)', expected 'r - f2' and 'a - f4'"
[ "$(ar -t a </dev/null)" = "$(printf 'f1\nf2\nf3\nf4')" ] || pc_fail "member order is wrong after replace and append"
[ "$(ar -p a f2 </dev/null)" = two-new ] || pc_fail "f2 was not replaced"
pc_pass

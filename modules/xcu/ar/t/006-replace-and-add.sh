# ar -r replaces in place and appends new files
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ar.006
# Replacing an existing member keeps its place in the archive and
# takes the new contents; a file not yet in the archive is appended
# at the end; with -v standard output has "r - NAME" for a replaced
# file and "a - NAME" for an added one.
. "$PC_CORE/pc.sh"
pc_begin XCU.ar.006
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
# Members are added one at a time so that the order does not depend on how
# several new operands are ordered.
pc_mk() { pc_a=$1; shift; for pc_f; do ar -rc "$pc_a" "$pc_f" </dev/null >/dev/null 2>&1 || return 1; done; }
printf 'one\n' >f1; printf 'two\n' >f2; printf 'three\n' >f3
pc_mk a f1 f2 f3 || pc_unresolved "setup: cannot build the archive"
printf 'two-new, longer than before\n' >f2; printf 'four\n' >f4; printf 'five\n' >f5
pc_test
ar -rv a f2 f4 >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "ar -rv: exit status $rc"
[ -f out ] || pc_fail "no output file"
# The order of the lines for several operands is not fixed.
sort out >out.s
printf 'a - f4\nr - f2\n' >exp
cmp -s out.s exp || pc_fail "ar -rv wrote '$(cat out)', expected 'r - f2' and 'a - f4'"
[ "$(ar -t a </dev/null | tr '\n' ' ')" = 'f1 f2 f3 f4 ' ] || pc_fail "member order is wrong after replace and append"
ar -p a f2 >o 2>/dev/null </dev/null
cmp -s o f2 || pc_fail "f2 was not replaced"
ar -p a f1 >o 2>/dev/null </dev/null
cmp -s o f1 || pc_fail "f1 changed"
ar -p a f3 >o 2>/dev/null </dev/null
cmp -s o f3 || pc_fail "f3 changed"
ar -p a f4 >o 2>/dev/null </dev/null
cmp -s o f4 || pc_fail "f4 has the wrong contents"
ar -rv a f1 >out 2>err </dev/null || pc_fail "ar -rv a f1: nonzero status"
printf 'r - f1\n' >exp
cmp -s out exp || pc_fail "ar -rv a f1 wrote '$(cat out)'"
ar -rv a f5 >out 2>err </dev/null || pc_fail "ar -rv a f5: nonzero status"
printf 'a - f5\n' >exp
cmp -s out exp || pc_fail "ar -rv a f5 wrote '$(cat out)'"
[ "$(ar -t a </dev/null | tr '\n' ' ')" = 'f1 f2 f3 f4 f5 ' ] || pc_fail "member order is wrong after the last additions"
pc_pass

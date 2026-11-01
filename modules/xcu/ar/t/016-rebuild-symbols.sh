# ar -s is accepted
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ar.016
# "ar -s a" on an existing archive exits 0 and the members are
# unchanged and still listed in order.
# MUTANT    ar.s_status1
# MUTANT    ar.s_empties
# MUTANT    ar.s_reorders
# MUTANT    ar.s_removes_archive
# MUTANT    ar.s_corrupts_content
. "$PC_CORE/pc.sh"
pc_begin XCU.ar.016
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
# Members are added one at a time so that the order does not depend on how
# several new operands are ordered.
pc_mk() { pc_a=$1; shift; for pc_f; do ar -rc "$pc_a" "$pc_f" </dev/null >/dev/null 2>&1 || return 1; done; }
printf 'one\n' >f1; printf 'two\n' >f2; printf 'odd' >f3
pc_mk a f2 f1 f3 || pc_unresolved "setup: cannot build the archive"
pc_test
ar -s a >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "ar -s: exit status $rc"
[ -f a ] || pc_fail "ar -s removed the archive"
[ "$(ar -t a </dev/null | tr '\n' ' ')" = 'f2 f1 f3 ' ] || pc_fail "the members changed after ar -s"
for f in f1 f2 f3; do
	ar -p a $f >o 2>/dev/null </dev/null
	cmp -s o $f || pc_fail "contents of $f changed after ar -s"
done
pc_pass

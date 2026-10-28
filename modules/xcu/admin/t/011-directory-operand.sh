# admin with a directory operand
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.admin.011
. "$PC_CORE/pc.sh"
pc_begin XCU.admin.011
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
for u in get admin prs; do command -v $u >/dev/null 2>&1 || pc_notinuse "$u is not available"; done
mkdir d || pc_unresolved "setup: mkdir"
printf 'one\n' >a; printf 'two\n' >b
admin -ia d/s.a </dev/null >setup.out 2>&1 || pc_unresolved "setup: admin d/s.a"
admin -ib d/s.b </dev/null >setup.out 2>&1 || pc_unresolved "setup: admin d/s.b"
printf 'junk\n' >d/junk || pc_unresolved "setup: junk"
pc_test
admin -fb d </dev/null >out 2>err || pc_fail "admin -fb on a directory: nonzero status: $(cat err)"
[ -s err ] && pc_fail "a diagnostic was written: $(cat err)"
for n in a b; do
	got=$(prs -r -d':BF:' d/s.$n </dev/null 2>/dev/null)
	[ "$got" = yes ] || pc_fail "d/s.$n does not have the b flag: '$got'"
done
[ "$(cat d/junk)" = junk ] || pc_fail "the non-SCCS file was altered"
pc_pass

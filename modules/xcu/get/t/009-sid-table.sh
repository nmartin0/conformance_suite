# which SID is read and which is made
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.get.009
# With no -r the latest delta is read and its successor made, -r with a
# higher release makes release.1, and -r of an earlier delta that has a
# successor makes a branch SID.
. "$PC_CORE/pc.sh"
pc_begin XCU.get.009
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
mk2() {
	printf 'one\ntwo\nthree\n' >"$1" || pc_unresolved "setup: cannot write $1"
	admin -i"$1" "s.$1" </dev/null >setup.out 2>&1 || pc_unresolved "setup: admin failed for s.$1"
	rm -f "$1"
	get -e -s "s.$1" </dev/null >setup.out 2>&1 || pc_unresolved "setup: get -e failed for s.$1"
	printf 'four\n' >>"$1" || pc_unresolved "setup: cannot edit $1"
	delta -s -y'add four' "s.$1" </dev/null >setup.out 2>&1 || pc_unresolved "setup: delta failed for s.$1"
}
mk2 a; mk2 b; mk2 c
pc_test
get -e s.a </dev/null >out 2>err || pc_fail "get -e: nonzero status"
printf '1.2\nnew delta 1.3\n4 lines\n' >exp
cmp out exp >/dev/null 2>&1 || pc_fail "with no -r: $(tr '\n' '|' <out)"
get -e -r2 s.b </dev/null >out 2>err || pc_fail "get -e -r2: nonzero status"
printf '1.2\nnew delta 2.1\n4 lines\n' >exp
cmp out exp >/dev/null 2>&1 || pc_fail "with -r2: $(tr '\n' '|' <out)"
get -e -r1.1 s.c </dev/null >out 2>err || pc_fail "get -e -r1.1: nonzero status"
printf '1.1\nnew delta 1.1.1.1\n3 lines\n' >exp
cmp out exp >/dev/null 2>&1 || pc_fail "with -r1.1 on a file with a successor: $(tr '\n' '|' <out)"
pc_pass

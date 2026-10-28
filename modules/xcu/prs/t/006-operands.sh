# directory and standard input operands
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.prs.006
# A directory operand processes each SCCS file in it and ignores other
# files, a - operand reads names from standard input, and a missing file
# gives a non-zero status.
. "$PC_CORE/pc.sh"
pc_begin XCU.prs.006
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
mk1() {
	printf 'one\ntwo\nthree\n' >"$1" || pc_unresolved "setup: cannot write $1"
	admin -i"$1" "s.$1" </dev/null >setup.out 2>&1 || pc_unresolved "setup: admin failed for s.$1"
	rm -f "$1"
}
mk1 a; mk1 b
printf 'plain\n' >notsccs
pc_test
prs -r -d':I:\n' . </dev/null >out 2>err || pc_fail "prs on a directory: nonzero status"
[ "$(cat out)" = '1.1
1.1' ] || pc_fail "directory operand gave: $(tr '\n' '|' <out)"
printf 's.a\ns.b\n' | prs -r -d':I:\n' - >out 2>err || pc_fail "prs -: nonzero status"
[ "$(cat out)" = '1.1
1.1' ] || pc_fail "standard input operand gave: $(tr '\n' '|' <out)"
prs -r -d':I:' nosuch.s >out 2>err </dev/null
[ "$?" -gt 0 ] || pc_fail "a missing file gave status 0"
pc_pass

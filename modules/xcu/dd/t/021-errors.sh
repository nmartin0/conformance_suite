# dd reports a missing input or an unwritable output
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.dd.021
. "$PC_CORE/pc.sh"
pc_begin XCU.dd.021
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
# A diagnostic (a line other than the two statistics forms) and a status
# above zero.
printf 'data\n' >in || pc_fail "cannot write in"
pc_test
diag() {
	[ -s err ] || return 1
	while IFS= read -r l; do
		case $l in
		*" records in" | *" records out") ;;
		*) return 0 ;;
		esac
	done <err
	return 1
}
dd if=nonexistent of=out </dev/null 2>err
rc=$?
[ "$rc" -gt 0 ] || pc_fail "dd of a missing input file exited with status $rc"
diag || pc_fail "dd of a missing input file wrote no diagnostic"
dd if=in of=nodir/out </dev/null 2>err
rc=$?
[ "$rc" -gt 0 ] || pc_fail "dd to a missing directory exited with status $rc"
diag || pc_fail "dd to a missing directory wrote no diagnostic"
pc_pass

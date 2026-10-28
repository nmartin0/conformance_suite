# dd ibs= and obs=
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.dd.004
. "$PC_CORE/pc.sh"
pc_begin XCU.dd.004
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
# Input is read in ibs blocks and rewritten in full obs blocks (the last
# one short); the counts follow the two sizes.
gen() { g=$1; while [ "$g" -lt "$2" ]; do printf 'L%02d-%095d\n' "$g" "$g"; g=$((g+1)); done; }
hasl() { while IFS= read -r hl; do [ "$hl" = "$2" ] && return 0; done <"$1"; return 1; }
gen 0 10 >f1000 || pc_fail "cannot write f1000"
pc_test
chk() {
	dd if=f1000 of=out "$1" "$2" </dev/null 2>err
	rc=$?
	[ "$rc" -eq 0 ] || pc_fail "dd $1 $2 exited with status $rc"
	cmp -s f1000 out || pc_fail "dd $1 $2: copy differs"
	hasl err "$3 records in" || pc_fail "dd $1 $2: no line '$3 records in'"
	hasl err "$4 records out" || pc_fail "dd $1 $2: no line '$4 records out'"
}
chk ibs=100 obs=300 10+0 3+1
chk ibs=300 obs=100 3+1 10+0
chk ibs=250 obs=250 4+0 4+0
pc_pass

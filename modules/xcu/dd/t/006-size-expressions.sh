# dd size expressions
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.dd.006
. "$PC_CORE/pc.sh"
pc_begin XCU.dd.006
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
# Sizes with k, b and x: the number of whole and partial input blocks
# shows which size dd used.
gen() { g=$1; while [ "$g" -lt "$2" ]; do printf 'L%02d-%095d\n' "$g" "$g"; g=$((g+1)); done; }
hasl() { while IFS= read -r hl; do [ "$hl" = "$2" ] && return 0; done <"$1"; return 1; }
gen 0 30 >f3000 || pc_fail "cannot write f3000"
gen 0 13 >f1300 || pc_fail "cannot write f1300"
gen 0 3 >f300 || pc_fail "cannot write f300"
gen 0 1 >f100 || pc_fail "cannot write f100"
pc_test
chk() {
	dd if="$2" of=out ibs="$1" </dev/null 2>err
	rc=$?
	[ "$rc" -eq 0 ] || pc_fail "dd ibs=$1 exited with status $rc"
	cmp -s "$2" out || pc_fail "dd ibs=$1: copy differs"
	hasl err "$3 records in" || pc_fail "dd ibs=$1 of $2: no line '$3 records in'"
}
chk 1k f3000 2+1
chk 1b f1300 2+1
chk 2b f1300 1+1
chk 2x5 f100 10+0
chk 2x3x5 f300 10+0
chk 2kx2 f3000 0+1
chk 1bx2 f3000 2+1
dd if=f3000 of=out ibs=100 obs=1k </dev/null 2>err
rc=$?
[ "$rc" -eq 0 ] || pc_fail "dd obs=1k exited with status $rc"
hasl err "2+1 records out" || pc_fail "dd obs=1k: no line '2+1 records out'"
pc_pass

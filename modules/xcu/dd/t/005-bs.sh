# dd bs= supersedes ibs= and obs=
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.dd.005
. "$PC_CORE/pc.sh"
pc_begin XCU.dd.005
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
# bs= sets both sizes and supersedes ibs= and obs=. The operands are given
# in the order ibs, obs, bs so that "bs wins" and "last wins" agree.
gen() { g=$1; while [ "$g" -lt "$2" ]; do printf 'L%02d-%095d\n' "$g" "$g"; g=$((g+1)); done; }
hasl() { while IFS= read -r hl; do [ "$hl" = "$2" ] && return 0; done <"$1"; return 1; }
gen 0 10 >f1000 || pc_fail "cannot write f1000"
pc_test
chk() {
	dd if=f1000 of=out ibs=7 obs=9 bs="$1" </dev/null 2>err
	rc=$?
	[ "$rc" -eq 0 ] || pc_fail "dd bs=$1 exited with status $rc"
	cmp -s f1000 out || pc_fail "dd bs=$1: copy differs"
	hasl err "$2 records in" || pc_fail "dd bs=$1: no line '$2 records in'"
	hasl err "$2 records out" || pc_fail "dd bs=$1: no line '$2 records out'"
}
chk 100 10+0
chk 300 3+1
pc_pass

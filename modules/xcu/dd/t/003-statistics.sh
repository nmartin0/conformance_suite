# dd statistics on standard error
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.dd.003
. "$PC_CORE/pc.sh"
pc_begin XCU.dd.003
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
# On success: status 0 and the lines "N+M records in" and "N+M records out"
# (512-byte blocks by default).
gen() { g=$1; while [ "$g" -lt "$2" ]; do printf 'L%02d-%095d\n' "$g" "$g"; g=$((g+1)); done; }
hasl() { while IFS= read -r hl; do [ "$hl" = "$2" ] && return 0; done <"$1"; return 1; }
gen 0 13 >f1300 || pc_fail "cannot write f1300"
gen 0 1 >f100 || pc_fail "cannot write f100"
: >f0 || pc_fail "cannot write f0"
# 1024 bytes exactly: 1000 bytes of gen plus 24 more.
{ gen 0 10; printf '%024d' 0; } >f1024 || pc_fail "cannot write f1024"
pc_test
chk() {
	dd if="$1" of=out </dev/null 2>err
	rc=$?
	[ "$rc" -eq 0 ] || pc_fail "dd of $1 exited with status $rc"
	hasl err "$2 records in" || pc_fail "$1: no line '$2 records in' on standard error"
	hasl err "$2 records out" || pc_fail "$1: no line '$2 records out' on standard error"
	cmp -s "$1" out || pc_fail "$1: copy differs"
}
chk f1300 2+1
chk f1024 2+0
chk f100 0+1
chk f0 0+0
pc_pass

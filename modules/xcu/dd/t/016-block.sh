# dd conv=block
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.dd.016
. "$PC_CORE/pc.sh"
pc_begin XCU.dd.016
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
# Records to fixed cbs length: newline dropped, space padding, truncation,
# the truncated-record report; independent of the input block size.
hasl() { while IFS= read -r hl; do [ "$hl" = "$2" ] && return 0; done <"$1"; return 1; }
printf 'ab\ncdefgh\nx' >f1 || pc_fail "cannot write f1"
printf 'ab   cdefgx    ' >e1 || pc_fail "cannot write e1"
printf 'abcdefg\nhijklmn\n' >f2 || pc_fail "cannot write f2"
printf 'abcdehijkl' >e2 || pc_fail "cannot write e2"
printf 'ab\n\ncd\n' >f3 || pc_fail "cannot write f3"
printf 'ab      cd  ' >e3 || pc_fail "cannot write e3"
pc_test
run() {
	dd if="$1" of=out cbs="$3" conv=block "$4" </dev/null 2>err
	rc=$?
	[ "$rc" -eq 0 ] || pc_fail "dd conv=block cbs=$3 $4 of $1 exited with status $rc"
	cmp -s "$2" out || pc_fail "dd conv=block cbs=$3 $4 of $1: wrong output data"
}
for ib in ibs=512 ibs=3; do
	run f1 e1 5 $ib
	hasl err "1 truncated record" || pc_fail "$ib: no line '1 truncated record'"
	run f2 e2 5 $ib
	hasl err "2 truncated records" || pc_fail "$ib: no line '2 truncated records'"
	run f3 e3 4 $ib
done
pc_pass

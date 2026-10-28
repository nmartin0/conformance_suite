# dd conv=sync
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.dd.015
. "$PC_CORE/pc.sh"
pc_begin XCU.dd.015
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
# A short input block is padded to ibs with NULs (spaces with block or
# unblock) and the padding is processed like input.
hasl() { while IFS= read -r hl; do [ "$hl" = "$2" ] && return 0; done <"$1"; return 1; }
printf 'abcde' >f5 || pc_fail "cannot write f5"
printf 'abcde\000\000\000' >e5 || pc_fail "cannot write e5"
printf '0123456789ABCDEFGHIJKLMNO' >f25 || pc_fail "cannot write f25"
printf '0123456789ABCDEFGHIJKLMNO\000\000\000\000\000' >e25 || pc_fail "cannot write e25"
printf 'abcdefghij' >f10 || pc_fail "cannot write f10"
printf 'abc' >f3 || pc_fail "cannot write f3"
printf 'abc\n\n' >eub || pc_fail "cannot write eub"
printf 'ab\n' >fbl || pc_fail "cannot write fbl"
printf 'ab      ' >ebl || pc_fail "cannot write ebl"
pc_test
run() {
	f=$1
	w=$2
	shift 2
	dd if="$f" of=out "$@" </dev/null 2>err
	rc=$?
	[ "$rc" -eq 0 ] || pc_fail "dd $* exited with status $rc"
	cmp -s "$w" out || pc_fail "dd $*: wrong output data"
}
run f5 e5 ibs=8 conv=sync
hasl err "0+1 records in" || pc_fail "ibs=8 conv=sync: no line '0+1 records in'"
hasl err "0+1 records out" || pc_fail "ibs=8 conv=sync: no line '0+1 records out'"
run f5 e5 bs=8 conv=sync
hasl err "0+1 records in" || pc_fail "bs=8 conv=sync: no line '0+1 records in'"
hasl err "1+0 records out" || pc_fail "bs=8 conv=sync: no line '1+0 records out'"
run f25 e25 ibs=10 obs=25 conv=sync
hasl err "2+1 records in" || pc_fail "ibs=10 obs=25 conv=sync: no line '2+1 records in'"
hasl err "1+1 records out" || pc_fail "ibs=10 obs=25 conv=sync: no line '1+1 records out'"
run f10 f10 ibs=5 conv=sync
run f3 eub ibs=6 cbs=4 conv=sync,unblock
run fbl ebl ibs=6 cbs=4 conv=sync,block
pc_pass

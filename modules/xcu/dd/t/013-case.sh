# dd conv=lcase and conv=ucase
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.dd.013
. "$PC_CORE/pc.sh"
pc_begin XCU.dd.013
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
# ASCII letters only (POSIX locale); the bytes just outside the letter
# ranges are in the data to catch off-by-one mappings.
printf 'Hello, World! 123 @AZ[ `az{ ~_\n' >in || pc_fail "cannot write in"
printf 'hello, world! 123 @az[ `az{ ~_\n' >explo || pc_fail "cannot write explo"
printf 'HELLO, WORLD! 123 @AZ[ `AZ{ ~_\n' >expup || pc_fail "cannot write expup"
pc_test
chk() {
	dd conv="$1" "$3" <in >out 2>err
	rc=$?
	[ "$rc" -eq 0 ] || pc_fail "dd conv=$1 $3 exited with status $rc"
	cmp -s "$2" out || pc_fail "dd conv=$1 $3 gave the wrong data"
}
chk lcase explo ibs=512
chk ucase expup ibs=512
chk lcase explo ibs=3
chk ucase expup ibs=3
pc_pass

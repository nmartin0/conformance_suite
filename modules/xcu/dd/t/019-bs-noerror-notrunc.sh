# dd bs= with noerror and notrunc
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.dd.019
. "$PC_CORE/pc.sh"
pc_begin XCU.dd.019
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
# bs= with only noerror and notrunc: the data is copied as it is, the old
# tail of a longer output file stays, the counts follow bs.
hasl() { while IFS= read -r hl; do [ "$hl" = "$2" ] && return 0; done <"$1"; return 1; }
rep() { r=0; while [ "$r" -lt "$1" ]; do printf '%s' "$2"; r=$((r+1)); done; }
rep 250 x >in || pc_fail "cannot write in"
rep 400 o >out || pc_fail "cannot write out"
{ rep 250 x; rep 150 o; } >exp || pc_fail "cannot write exp"
pc_test
dd if=in of=out bs=100 conv=noerror,notrunc </dev/null 2>err
rc=$?
[ "$rc" -eq 0 ] || pc_fail "dd bs=100 conv=noerror,notrunc exited with status $rc"
cmp -s exp out || pc_fail "bs=100 conv=noerror,notrunc: wrong output data"
hasl err "2+1 records in" || pc_fail "no line '2+1 records in'"
hasl err "2+1 records out" || pc_fail "no line '2+1 records out'"
pc_pass

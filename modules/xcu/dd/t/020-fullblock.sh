# dd iflags=fullblock
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.dd.020
. "$PC_CORE/pc.sh"
pc_begin XCU.dd.020
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
# A slow pipe gives short reads; fullblock makes dd collect full blocks, and
# count= then counts full blocks. The writer pauses to force short reads.
hasl() { while IFS= read -r hl; do [ "$hl" = "$2" ] && return 0; done <"$1"; return 1; }
printf 'abcdefghij' >exp1 || pc_fail "cannot write exp1"
printf 'abcdefgh' >exp2 || pc_fail "cannot write exp2"
pc_test
{ printf ab; sleep 1; printf cdef; sleep 1; printf ghij; } 2>/dev/null | dd ibs=4 iflags=fullblock of=out 2>err
rc=$?
[ "$rc" -eq 0 ] || pc_fail "dd iflags=fullblock exited with status $rc"
cmp -s exp1 out || pc_fail "iflags=fullblock: the data is wrong"
hasl err "2+1 records in" || pc_fail "iflags=fullblock: no line '2+1 records in'"
hasl err "0+1 records out" || pc_fail "iflags=fullblock: no line '0+1 records out'"
{ printf ab; sleep 1; printf cdefghij; } 2>/dev/null | dd ibs=4 count=2 iflags=fullblock of=out 2>err
rc=$?
[ "$rc" -eq 0 ] || pc_fail "dd count=2 iflags=fullblock exited with status $rc"
cmp -s exp2 out || pc_fail "count=2 iflags=fullblock: did not copy two full blocks"
hasl err "2+0 records in" || pc_fail "count=2 iflags=fullblock: no line '2+0 records in'"
pc_pass

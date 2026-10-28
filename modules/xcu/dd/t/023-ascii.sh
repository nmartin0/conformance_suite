# dd conv=ascii
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.dd.023
. "$PC_CORE/pc.sh"
pc_begin XCU.dd.023
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
# EBCDIC to ASCII, then unblock.
printf '\201\301\360\232\137\100' >in || pc_fail "cannot write in"
printf 'aA0^~\n' >exp || pc_fail "cannot write exp"
pc_test
dd if=in of=out cbs=6 conv=ascii </dev/null 2>err
rc=$?
[ "$rc" -eq 0 ] || pc_fail "dd conv=ascii exited with status $rc"
cmp -s exp out || pc_fail "conv=ascii: wrong bytes"
pc_pass

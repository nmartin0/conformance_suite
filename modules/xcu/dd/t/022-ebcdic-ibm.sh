# dd conv=ebcdic and conv=ibm
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.dd.022
. "$PC_CORE/pc.sh"
pc_begin XCU.dd.022
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
# Block then convert; the standard and IBM tables differ for ^ and ~.
printf 'aA0^~\n' >in || pc_fail "cannot write in"
printf '\201\301\360\232\137\100' >eeb || pc_fail "cannot write eeb"
printf '\201\301\360\137\241\100' >eib || pc_fail "cannot write eib"
pc_test
dd if=in of=out cbs=6 conv=ebcdic </dev/null 2>err
rc=$?
[ "$rc" -eq 0 ] || pc_fail "dd conv=ebcdic exited with status $rc"
cmp -s eeb out || pc_fail "conv=ebcdic: wrong bytes"
dd if=in of=out cbs=6 conv=ibm </dev/null 2>err
rc=$?
[ "$rc" -eq 0 ] || pc_fail "dd conv=ibm exited with status $rc"
cmp -s eib out || pc_fail "conv=ibm: wrong bytes"
pc_pass

# dd count=
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.dd.007
. "$PC_CORE/pc.sh"
pc_begin XCU.dd.007
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
# count=n copies only n input blocks.
gen() { g=$1; while [ "$g" -lt "$2" ]; do printf 'L%02d-%095d\n' "$g" "$g"; g=$((g+1)); done; }
hasl() { while IFS= read -r hl; do [ "$hl" = "$2" ] && return 0; done <"$1"; return 1; }
gen 0 10 >f1000 || pc_fail "cannot write f1000"
gen 0 2 >exp2 || pc_fail "cannot write exp2"
gen 0 3 >exp3 || pc_fail "cannot write exp3"
pc_test
dd if=f1000 of=out ibs=100 count=2 </dev/null 2>err
rc=$?
[ "$rc" -eq 0 ] || pc_fail "dd count=2 exited with status $rc"
cmp -s exp2 out || pc_fail "count=2 ibs=100 did not copy exactly the first 200 bytes"
hasl err "2+0 records in" || pc_fail "no line '2+0 records in'"
hasl err "0+1 records out" || pc_fail "no line '0+1 records out'"
dd if=f1000 of=out ibs=300 count=1 </dev/null 2>err
rc=$?
[ "$rc" -eq 0 ] || pc_fail "dd count=1 exited with status $rc"
cmp -s exp3 out || pc_fail "count=1 ibs=300 did not copy exactly the first 300 bytes"
hasl err "1+0 records in" || pc_fail "no line '1+0 records in'"
pc_pass

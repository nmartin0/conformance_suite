# hash -r empties the list.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.hash.002
. "$PC_CORE/pc.sh"
pc_begin XCU.hash.002
mkdir bin1 || pc_fail "cannot make a directory"
printf 'exit 0\n' >bin1/pchashx || pc_fail "cannot write a script"
chmod +x bin1/pchashx || pc_fail "cannot make the script executable"
PATH=$PWD/bin1:$PATH
hash pchashx >/dev/null 2>&1 || pc_fail "hash pchashx failed"
pchashx >/dev/null 2>&1
hash >out 2>/dev/null
case $(cat out) in
*/bin1/pchashx*) ;;
*) pc_fail "utility was not reported before hash -r" ;;
esac
pc_test
hash -r
rc=$?
[ "$rc" -eq 0 ] || pc_fail "hash -r exited $rc"
hash >out 2>/dev/null
report=$(cat out)
case $report in
*pchashx*) pc_fail "utility still reported after hash -r: '$report'" ;;
esac
pc_pass

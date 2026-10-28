# Changing PATH clears the remembered locations.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.hash.004
. "$PC_CORE/pc.sh"
pc_begin XCU.hash.004
mkdir bin1 d2 || pc_fail "cannot make directories"
printf 'exit 0\n' >bin1/pchashx || pc_fail "cannot write a script"
chmod +x bin1/pchashx || pc_fail "cannot make the script executable"
PATH=$PWD/bin1:$PATH
hash pchashx >/dev/null 2>&1 || pc_fail "hash pchashx failed"
pchashx >/dev/null 2>&1
hash >out 2>/dev/null
case $(cat out) in
*/bin1/pchashx*) ;;
*) pc_fail "utility was not reported before the PATH change" ;;
esac
pc_test
PATH=$PWD/d2:$PATH
hash >out 2>/dev/null
report=$(cat out)
case $report in
*pchashx*) pc_fail "utility still reported after PATH changed: '$report'" ;;
esac
pc_pass

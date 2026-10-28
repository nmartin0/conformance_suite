# hash adds a utility's location and a bare hash reports it.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.hash.001
. "$PC_CORE/pc.sh"
pc_begin XCU.hash.001
mkdir bin1 || pc_fail "cannot make a directory"
printf 'exit 0\n' >bin1/pchashx || pc_fail "cannot write a script"
chmod +x bin1/pchashx || pc_fail "cannot make the script executable"
PATH=$PWD/bin1:$PATH
pc_test
hash pchashx >out 2>/dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "hash pchashx exited $rc"
[ ! -s out ] || pc_fail "hash with an operand wrote to standard output"
# the list is described as holding utilities that have been invoked, so run it
pchashx >/dev/null 2>&1
hash >out 2>/dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "hash with no operands exited $rc"
report=$(cat out)
case $report in
*/bin1/pchashx*) ;;
*) pc_fail "report does not contain the pathname: '$report'" ;;
esac
pc_pass

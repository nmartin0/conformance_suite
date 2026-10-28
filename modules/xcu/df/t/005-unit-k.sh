# The total in 1024-byte units is the total in 512-byte units rounded up to an even number divided by two.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.df.005
. "$PC_CORE/pc.sh"
pc_begin XCU.df.005
pc_test
df -P . >out 2>err </dev/null
set -- $(sed -n 2p out)
[ "$#" -eq 6 ] || pc_unresolved "unexpected number of fields ($#) in the data line"
t512=$2
df -k -P . >out 2>err </dev/null
set -- $(sed -n 2p out)
[ "$#" -eq 6 ] || pc_unresolved "unexpected number of fields ($#) in the data line"
t1024=$2
case $t512$t1024 in
''|*[!0-9]*) pc_fail "totals '$t512' and '$t1024' are not numbers" ;;
esac
[ "$t1024" -eq $(((t512 + 1) / 2)) ] || pc_fail "total is $t512 in 512-byte units but $t1024 in 1024-byte units"
pc_pass

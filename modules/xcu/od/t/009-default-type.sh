# With no output type od behaves as with -t oS.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.od.009
. "$PC_CORE/pc.sh"
pc_begin XCU.od.009
LC_ALL=POSIX
export LC_ALL
NL='
'
printf 'aaaabbbbccccdddd\n' >f1 || pc_fail "cannot create input"
pc_test
od -A n -v f1 >out1 2>/dev/null </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "od f1 gave status $rc"
od -A n -v -t oS f1 >out2 2>/dev/null </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "od -t oS f1 gave status $rc"
a=$(cat out1; printf x)
b=$(cat out2; printf x)
[ "$a" = "$b" ] || pc_fail "the default output differs from -t oS"
[ "$a" != x ] || pc_fail "no output"
pc_pass

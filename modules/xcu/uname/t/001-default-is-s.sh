# uname and uname -s write the same.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.uname.001
. "$PC_CORE/pc.sh"
pc_begin XCU.uname.001
LC_ALL=C; export LC_ALL
pc_test
uname >o1 2>/dev/null </dev/null || pc_fail "uname failed"
uname -s >o2 2>/dev/null </dev/null || pc_fail "uname -s failed"
a=$(cat o1; printf x)
b=$(cat o2; printf x)
[ "$a" != x ] || pc_fail "uname wrote nothing"
[ "$a" = "$b" ] || pc_fail "uname wrote '${a%x}', uname -s wrote '${b%x}'"
pc_pass

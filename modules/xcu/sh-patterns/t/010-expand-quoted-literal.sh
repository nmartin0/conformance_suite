# Quoted or escaped pattern characters are not expanded as filename
# patterns.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.sh-patterns.010
. "$PC_CORE/pc.sh"
pc_begin XCU.sh-patterns.010
mkdir pc.fe || pc_unresolved "mkdir"
: >pc.fe/a
pc_test
r=$(cd pc.fe && echo "*")
[ "$r" = '*' ] || pc_fail "double-quoted *: '$r'"
r=$(cd pc.fe && echo '*')
[ "$r" = '*' ] || pc_fail "single-quoted *: '$r'"
r=$(cd pc.fe && echo \*)
[ "$r" = '*' ] || pc_fail "escaped *: '$r'"
r=$(cd pc.fe && pcq='*'; echo "$pcq")
[ "$r" = '*' ] || pc_fail "quoted expansion of *: '$r'"
r=$(cd pc.fe && pcq='*'; echo $pcq)
[ "$r" = a ] || pc_fail "unquoted expansion of * did not expand: '$r'"
pc_pass

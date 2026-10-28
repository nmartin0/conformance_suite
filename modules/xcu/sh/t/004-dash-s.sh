# sh -s reads commands from standard input and takes its operands as
# positional parameters; with no operands and no -c, -s is assumed.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.sh.004
. "$PC_CORE/pc.sh"
pc_begin XCU.sh.004
pc_test
r=$(printf 'echo "$1 $2"\n' | ${PC_SH:-sh} -s a b)
[ "$r" = "a b" ] || pc_fail "${PC_SH:-sh} -s a b gave '$r'"
r=$(printf 'echo hi\n' | ${PC_SH:-sh})
[ "$r" = hi ] || pc_fail "${PC_SH:-sh} with no operands gave '$r'"
r=$(printf 'echo "$#"\n' | ${PC_SH:-sh} -s)
[ "$r" = 0 ] || pc_fail "${PC_SH:-sh} -s with no operands set \$# to '$r'"
pc_pass

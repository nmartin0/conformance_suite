# set with operands replaces all positional parameters and $# with them;
# earlier ones that are not replaced become unset.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.set.001
. "$PC_CORE/pc.sh"
pc_begin XCU.set.001
pc_test
set a b c
[ "$#" -eq 3 ] && [ "$1" = a ] && [ "$2" = b ] && [ "$3" = c ] || pc_fail "set a b c: \$#=$# params '$*'"
set x
[ "$#" -eq 1 ] && [ "$1" = x ] || pc_fail "set x: \$#=$# params '$*'"
[ -z "${2+set}" ] && [ -z "${3+set}" ] || pc_fail "old parameters 2 and 3 still set"
pc_pass

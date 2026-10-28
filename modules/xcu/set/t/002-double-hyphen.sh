# set -- with no operands unsets all positional parameters; after --,
# operands that begin with - or + are plain parameters and set no options.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.set.002
. "$PC_CORE/pc.sh"
pc_begin XCU.set.002
pc_test
set a b
set --
[ "$#" -eq 0 ] && [ -z "${1+set}" ] || pc_fail "set -- left \$#=$#"
set -- -x -y +z
[ "$#" -eq 3 ] && [ "$1" = -x ] && [ "$2" = -y ] && [ "$3" = +z ] || pc_fail "parameters are '$*'"
case $- in
*x*) pc_fail "set -- -x turned on the x option" ;;
esac
x='-n'
set -- "$x"
[ "$#" -eq 1 ] && [ "$1" = -n ] || pc_fail "set -- \"\$x\" gave '$*'"
pc_pass

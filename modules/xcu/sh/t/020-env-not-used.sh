# A non-interactive shell does not read the file named by ENV.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.sh.020
. "$PC_CORE/pc.sh"
pc_begin XCU.sh.020
printf '%s\n' 'echo envrun' >pc.envf || pc_unresolved "write"
pc_test
r=$(ENV="$PWD/pc.envf" ${PC_SH:-sh} -c 'echo main' </dev/null)
[ "$r" = main ] || pc_fail "${PC_SH:-sh} -c: got '$r'"
r=$(printf 'echo main\n' | ENV="$PWD/pc.envf" ${PC_SH:-sh})
[ "$r" = main ] || pc_fail "${PC_SH:-sh} from standard input: got '$r'"
pc_pass

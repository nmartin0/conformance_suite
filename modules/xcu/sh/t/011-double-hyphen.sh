# sh accepts -- as the end of options, before the command string or the
# command file.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.sh.011
. "$PC_CORE/pc.sh"
pc_begin XCU.sh.011
printf '%s\n' 'echo "$1"' >pc.cf || pc_unresolved "write"
pc_test
r=$(${PC_SH:-sh} -c -- 'echo "$1"' pcname x </dev/null)
[ "$r" = x ] || pc_fail "${PC_SH:-sh} -c -- got '$r'"
r=$(${PC_SH:-sh} -- pc.cf y </dev/null)
[ "$r" = y ] || pc_fail "${PC_SH:-sh} -- file got '$r'"
pc_pass

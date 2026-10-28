# A utility starts in the shell's working directory with the shell's file
# creation mask and with descriptors added by redirections of its command.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.sh-environment.002
. "$PC_CORE/pc.sh"
pc_begin XCU.sh-environment.002
mkdir pc.sub || pc_unresolved "mkdir"
printf 'line\n' >pc.in || pc_unresolved "write"
pc_test
d1=$(cd pc.sub && ${PC_SH:-sh} -c 'pwd -P' </dev/null)
d2=$(cd pc.sub && pwd -P)
[ "$d1" = "$d2" ] || pc_fail "working directory: utility saw '$d1', expected '$d2'"
m0=$(umask)
umask 027
m1=$(${PC_SH:-sh} -c umask </dev/null)
m2=$(umask)
umask "$m0"
[ "$m1" = "$m2" ] || pc_fail "file creation mask: utility saw '$m1', shell has '$m2'"
r=$(${PC_SH:-sh} -c 'read x <&3; echo "$x"' 3<pc.in)
[ "$r" = line ] || pc_fail "descriptor 3 from a redirection: got '$r'"
pc_pass

# What a utility changes in its own environment (directory, mask, variables)
# does not change the shell's environment.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.sh-environment.003
. "$PC_CORE/pc.sh"
pc_begin XCU.sh-environment.003
pc_test
d0=$(pwd -P)
m0=$(umask)
unset PCZ
${PC_SH:-sh} -c 'cd /; PCZ=1; umask 0' </dev/null
[ "$(pwd -P)" = "$d0" ] || pc_fail "working directory changed"
[ "$(umask)" = "$m0" ] || pc_fail "mask changed"
[ -z "${PCZ+set}" ] || pc_fail "variable appeared"
pc_pass

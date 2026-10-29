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
# the utility does change its own environment (it records what it saw)
${PC_SH:-sh} -c 'cd /; PCZ=1; umask 0; printf "%s %s\n" "$(pwd -P)" "$PCZ" >"$1"' pcname "$d0/pc.rec" </dev/null
rec=
[ -f pc.rec ] && read -r rec <pc.rec
[ "$rec" = "/ 1" ] || pc_fail "the utility did not run or did not see its own changes (recorded '$rec')"
[ "$(pwd -P)" = "$d0" ] || pc_fail "working directory changed"
[ "$(umask)" = "$m0" ] || pc_fail "mask changed"
[ -z "${PCZ+set}" ] || pc_fail "variable appeared"
pc_pass

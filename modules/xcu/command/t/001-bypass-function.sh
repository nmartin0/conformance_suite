# command runs the utility, not a function of the same name.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.command.001
. "$PC_CORE/pc.sh"
pc_begin XCU.command.001
mkdir bin || pc_fail "cannot make a directory"
printf 'echo UTIL "$@"\nexit 7\n' >bin/pccmd || pc_fail "cannot write a script"
chmod +x bin/pccmd || pc_fail "cannot make the script executable"
PATH=$PWD/bin:$PATH
pccmd() { echo FUNC; return 3; }
got=$(pccmd)
[ "$got" = FUNC ] || pc_fail "setup: the function was not called by name"
pc_test
command pccmd a b >out 2>/dev/null
rc=$?
got=$(cat out)
[ "$got" = 'UTIL a b' ] || pc_fail "command pccmd a b gave '$got', wanted 'UTIL a b'"
[ "$rc" -eq 7 ] || pc_fail "command pccmd exited $rc, wanted 7 (the utility's status)"
pc_pass

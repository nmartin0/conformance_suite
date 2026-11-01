# command runs the utility, not a function of the same name.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.command.001
# MUTANT    command.fn_called
# MUTANT    command.status_zero
# MUTANT    command.status_one
# MUTANT    command.args_dropped
# MUTANT    command.args_swapped
# MUTANT    command.args_split
# MUTANT    command.empty_arg_dropped
#
# The utility prints each argument in brackets, so that a dropped,
# re-split or reordered argument shows.
. "$PC_CORE/pc.sh"
pc_begin XCU.command.001
mkdir bin || pc_fail "cannot make a directory"
printf 'printf "UTIL"\nprintf "[%%s]" "$@"\nprintf "\\n"\nexit 7\n' >bin/pccmd || pc_fail "cannot write a script"
chmod +x bin/pccmd || pc_fail "cannot make the script executable"
PATH=$PWD/bin:$PATH
pccmd() { echo FUNC; return 3; }
got=$(pccmd)
[ "$got" = FUNC ] || pc_fail "setup: the function was not called by name"
pc_test
command pccmd a 'b  c' '' >out 2>/dev/null
rc=$?
pc_need_file out "the output of the utility"
got=$(cat out)
want='UTIL[a][b  c][]'
[ "$got" = "$want" ] || pc_fail "command pccmd a 'b  c' '' gave '$got', wanted '$want'"
[ "$rc" -eq 7 ] || pc_fail "command pccmd exited $rc, wanted 7 (the utility's status)"
pc_pass

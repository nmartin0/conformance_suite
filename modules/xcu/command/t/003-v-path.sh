# command -v writes the absolute pathname of a utility.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.command.003
# MUTANT    command.v_name_only
# MUTANT    command.v_relative
# MUTANT    command.v_first_dir
# MUTANT    command.v_no_newline
# MUTANT    command.v_extra_line
# MUTANT    command.v_runs
# MUTANT    command.v_status1
# MUTANT    command.v_to_stderr
# VARIANT   command.v_dot_components
#
# The output is read back with read, which tells a missing final newline
# and extra output from a clean "%s\n"; a command substitution would hide
# both. The utility sits in the second PATH directory, so a wrong
# directory shows.
. "$PC_CORE/pc.sh"
pc_begin XCU.command.003
mkdir bin empty || pc_fail "cannot make a directory"
printf ': >ran\n' >bin/pccmd || pc_fail "cannot write a script"
chmod +x bin/pccmd || pc_fail "cannot make the script executable"
base=$PWD
PATH=$base/nodir:$base/empty:$base/bin:$PATH
pc_test
command -v pccmd >out 2>/dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "command -v exited $rc"
pc_need_file out "the output of command -v"
{ IFS= read -r l1; r1=$?; IFS= read -r l2; r2=$?; } <out
[ "$r1" -eq 0 ] || pc_fail "command -v wrote '$l1' without a final newline (or nothing)"
[ "$r2" -ne 0 ] && [ -z "$l2" ] || pc_fail "command -v wrote more than one line"
case $l1 in
/*) ;;
*) pc_fail "command -v wrote '$l1', not an absolute pathname" ;;
esac
[ "$l1" -ef "$base/bin/pccmd" ] || pc_fail "command -v wrote '$l1', which is not the utility $base/bin/pccmd"
[ ! -e ran ] || pc_fail "command -v ran the utility"
pc_pass

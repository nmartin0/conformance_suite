# command -v writes a line carrying an alias's name and value.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.command.006
# MUTANT    command.alias_name_only
# MUTANT    command.alias_value_only
# MUTANT    command.alias_silent
# MUTANT    command.alias_status1
# MUTANT    command.alias_no_newline
# VARIANT   command.alias_dashdash
# VARIANT   command.alias_dquote
#
# The page does not fix the line's format, only that it is a command line
# representing the definition. The value is a single plain word, so that
# any way of quoting it still shows it unbroken.
. "$PC_CORE/pc.sh"
pc_begin XCU.command.006
alias pcal6=pcvalue6
pc_test
command -v pcal6 >out 2>/dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "command -v for an alias exited $rc"
pc_need_file out "the output of command -v"
{ IFS= read -r l1; r1=$?; IFS= read -r l2; r2=$?; } <out
[ "$r1" -eq 0 ] || pc_fail "command -v for an alias wrote '$l1' without a final newline (or nothing)"
[ "$r2" -ne 0 ] && [ -z "$l2" ] || pc_fail "command -v for an alias wrote more than one line"
case $l1 in
*pcal6*) ;;
*) pc_fail "command -v for an alias wrote '$l1', without the name" ;;
esac
case $l1 in
*pcvalue6*) ;;
*) pc_fail "command -v for an alias wrote '$l1', without the value" ;;
esac
pc_pass

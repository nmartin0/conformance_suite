# The header text of an -o name is the rest of the argument, blanks included.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ps.016
# MUTANT    ps.text_cut_blank
# MUTANT    ps.text_ignored
# VARIANT   ps.padded
. "$PC_CORE/pc.sh"
pc_begin XCU.ps.016
LC_ALL=C; export LC_ALL
pc_test
ps -o "pid=Process ID" -p "$$" >out 2>err </dev/null || pc_fail "ps -o 'pid=Process ID' failed"
read -r h <out
[ "$h" = "Process ID" ] || pc_fail "heading is '$h', expected 'Process ID'"
ps -o "user=User Name" -o pid=Process\ ID -p "$$" >out 2>err </dev/null || pc_fail "ps with two blank headings failed"
read -r h <out
case $h in
"User Name "*"Process ID") ;;
*) pc_fail "heading line is '$h', expected 'User Name' then 'Process ID'" ;;
esac
pc_pass

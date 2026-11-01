# alias for a missing name fails with no output.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.alias.005
# MUTANT    alias.missing_status0
# MUTANT    alias.missing_stdout_diag
# MUTANT    alias.missing_empty_stdout
# MUTANT    alias.missing_prints_def
# MUTANT    alias.missing_status_last
# MUTANT    alias.missing_status_first
# MUTANT    alias.missing_prefix
# MUTANT    alias.missing_case_fold
# VARIANT   alias.missing_status2
# VARIANT   alias.missing_status255
#
# Any status above zero will do (variants 2 and 255). A missing name among
# existing ones still makes the status non-zero, whichever operand it is.
. "$PC_CORE/pc.sh"
pc_begin XCU.alias.005
pc_test
alias pcal5='echo five'
alias pcal5xy='echo xy'
alias pcnosuch1 >out 2>/dev/null
rc=$?
[ "$rc" -gt 0 ] || pc_fail "alias for a missing name exited $rc"
pc_need_file out "the output"
[ ! -s out ] || pc_fail "alias for a missing name wrote to standard output"
# an existing name before the missing one
alias pcal5 pcnosuch1 >out1 2>/dev/null
rc=$?
[ "$rc" -gt 0 ] || pc_fail "alias for an existing and then a missing name exited $rc"
pc_need_file out1 "the output"
while IFS= read -r line; do
	case $line in
	pcnosuch1*) pc_fail "a line was written for the missing name: $line" ;;
	esac
done <out1
# an existing name after the missing one
alias pcnosuch1 pcal5 >out2 2>/dev/null
rc=$?
[ "$rc" -gt 0 ] || pc_fail "alias for a missing and then an existing name exited $rc"
pc_need_file out2 "the output"
while IFS= read -r line; do
	case $line in
	pcnosuch1*) pc_fail "a line was written for the missing name: $line" ;;
	esac
done <out2
# a name that only begins like an existing one is a missing name
alias pcal5x >out3 2>/dev/null
rc=$?
[ "$rc" -gt 0 ] || pc_fail "alias for a name that is a prefix of another exited $rc"
pc_need_file out3 "the output"
[ ! -s out3 ] || pc_fail "alias for a prefix of another name wrote to standard output"
# names are compared as they are written
alias PCAL5 >out4 2>/dev/null
rc=$?
[ "$rc" -gt 0 ] || pc_fail "alias for the name in capitals exited $rc"
pc_need_file out4 "the output"
[ ! -s out4 ] || pc_fail "alias for the name in capitals wrote to standard output"
pc_pass

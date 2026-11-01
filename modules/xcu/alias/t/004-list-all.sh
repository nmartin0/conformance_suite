# alias with no operands writes every definition.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.alias.004
# MUTANT    alias.list_first_only
# MUTANT    alias.list_last_only
# MUTANT    alias.list_skip_first
# MUTANT    alias.list_skip_last
# MUTANT    alias.list_no_value
# MUTANT    alias.list_exit1
# MUTANT    alias.list_stderr
# MUTANT    alias.list_empty_exit1
# MUTANT    alias.list_no_newline
# VARIANT   alias.list_reverse
# VARIANT   alias.list_predefined
# VARIANT   alias.quote_double
# VARIANT   alias.quote_backslash
#
# The order of the lines is not fixed, and aliases that exist beforehand
# may be listed; both are tolerated.
. "$PC_CORE/pc.sh"
pc_begin XCU.alias.004
nl='
'
pc_test
alias pcal4a='echo 1'
alias pcal4b='echo 2 2'
alias pcal4c='echo 3'
alias pcal4d='echo 4 "x"'
alias >out 2>/dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "alias with no operands exited $rc"
pc_need_file out "the output"
content=$(cat out; echo x)
content=${content%x}
case $content in
*"$nl") ;;
*) pc_fail "the listing does not end with a newline" ;;
esac
a= b= c= d=
while IFS= read -r line; do
	case $line in
	pcal4a=*) a=1; rest=${line#*=}; exp='echo 1' ;;
	pcal4b=*) b=1; rest=${line#*=}; exp='echo 2 2' ;;
	pcal4c=*) c=1; rest=${line#*=}; exp='echo 3' ;;
	pcal4d=*) d=1; rest=${line#*=}; exp='echo 4 "x"' ;;
	*) continue ;;
	esac
	res=$(eval "set -- $rest" 2>/dev/null; printf '%s|%s' "$#" "$1")
	[ "$res" = "1|$exp" ] || pc_fail "line '$line' does not give the value '$exp'"
done <out
[ -n "$a" ] || pc_fail "pcal4a not listed"
[ -n "$b" ] || pc_fail "pcal4b not listed"
[ -n "$c" ] || pc_fail "pcal4c not listed"
[ -n "$d" ] || pc_fail "pcal4d not listed"
# with no alias left, the listing is empty and the status is still zero
( unalias -a 2>/dev/null || exit 7; alias >out2 2>/dev/null; echo $? >rc2 )
rc=$?
[ "$rc" -ne 7 ] || pc_unresolved "unalias -a failed"
pc_need_file rc2 "the status"
read -r rc2 <rc2
[ "$rc2" = 0 ] || pc_fail "alias with no operands and no aliases exited $rc2"
pc_need_file out2 "the output"
[ ! -s out2 ] || pc_fail "alias with no aliases wrote output"
pc_pass

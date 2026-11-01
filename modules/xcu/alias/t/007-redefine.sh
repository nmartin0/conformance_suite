# A second definition replaces the first.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.alias.007
# MUTANT    alias.redefine_ignored
# MUTANT    alias.redefine_append
# MUTANT    alias.redefine_exit1
# MUTANT    alias.redefine_subst_old
# MUTANT    alias.redefine_shows_old
# MUTANT    alias.redefine_shows_both
# MUTANT    alias.list_old_kept
# VARIANT   alias.quote_double
# VARIANT   alias.quote_backslash
# VARIANT   alias.list_predefined
. "$PC_CORE/pc.sh"
pc_begin XCU.alias.007
# use TEXT: run TEXT through the shell, so that its aliases are looked up
use() { eval "$@"; }
nl='
'
# value_of_line TEXT: "count|first word" of the text after the first equals sign
value_of_line() {
	vr=${1#*=}
	(eval "set -- $vr" 2>/dev/null; printf '%s|%s' "$#" "$1")
}
pc_test
alias pcal7='echo one'
alias pcal7='echo two'
rc=$?
[ "$rc" -eq 0 ] || pc_fail "redefining an alias exited $rc"
use pcal7 >o1 2>/dev/null
got=$(cat o1)
[ "$got" = two ] || pc_fail "substituted '$got', wanted two"
alias pcal7 >o2 2>/dev/null
shown=$(cat o2)
case $shown in
*"$nl"*) pc_fail "alias pcal7 shows more than one line: $shown" ;;
esac
got=$(value_of_line "$shown")
[ "$got" = "1|echo two" ] || pc_fail "alias pcal7 does not show the new value: $shown"
# the listing keeps only the new definition
alias >lst 2>/dev/null
pc_need_file lst "the listing"
n=0
while IFS= read -r line; do
	case $line in
	pcal7=*one*) pc_fail "the listing still has the old definition: $line" ;;
	pcal7=*) n=$((n + 1)) ;;
	esac
done <lst
[ "$n" -eq 1 ] || pc_fail "the listing has $n lines for pcal7, wanted 1"
# from a long value to a short one
alias pcal7b='echo a rather long value, longer than the next one'
alias pcal7b=true
use pcal7b >o3 2>/dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "short redefinition does not run true: $rc"
alias pcal7b >o4 2>/dev/null
shown=$(cat o4)
got=$(value_of_line "$shown")
[ "$got" = "1|true" ] || pc_fail "alias pcal7b shows '$shown' after the short redefinition"
pc_pass

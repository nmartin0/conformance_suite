# Several operands in one call are all processed.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.alias.013
# MUTANT    alias.multi_define_first_only
# MUTANT    alias.multi_define_last_only
# MUTANT    alias.multi_show_first_only
# MUTANT    alias.multi_show_last_only
# MUTANT    alias.multi_mixed_no_show
# MUTANT    alias.multi_mixed_no_define
# VARIANT   alias.multi_show_reverse
# VARIANT   alias.quote_double
# VARIANT   alias.quote_backslash
#
# The text does not say in which order the definitions of several names are
# written, so the lines may come in either order.
. "$PC_CORE/pc.sh"
pc_begin XCU.alias.013
# use TEXT: run TEXT through the shell, so that its aliases are looked up
use() { eval "$@"; }
pc_test
alias pcm1='echo m1' pcm2='echo m2' pcm4='echo m4'
rc=$?
[ "$rc" -eq 0 ] || pc_fail "alias with three definitions exited $rc"
use pcm1 >g1 2>/dev/null
use pcm2 >g2 2>/dev/null
use pcm4 >g4 2>/dev/null
for k in 1 2 4; do
	got=$(cat "g$k")
	[ "$got" = "m$k" ] || pc_fail "alias number $k gives '$got'"
done
alias pcm1 pcm2 >out 2>/dev/null || pc_fail "alias with two names failed"
pc_need_file out "the output"
n1=0 n2=0 other=0
while IFS= read -r line; do
	case $line in
	pcm1=*) n1=$((n1 + 1)) ;;
	pcm2=*) n2=$((n2 + 1)) ;;
	*) other=$((other + 1)) ;;
	esac
done <out
[ "$n1" -eq 1 ] || pc_fail "pcm1 shown $n1 times"
[ "$n2" -eq 1 ] || pc_fail "pcm2 shown $n2 times"
[ "$other" -eq 0 ] || pc_fail "$other unexpected lines"
# a definition and a name in one call
alias pcm3='echo m3' pcm1 >out2 2>/dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "alias with a definition and a name exited $rc"
use pcm3 >g3 2>/dev/null
got=$(cat g3)
[ "$got" = m3 ] || pc_fail "definition given with a name gives '$got'"
pc_need_file out2 "the output"
n1=0 other=0
while IFS= read -r line; do
	case $line in
	pcm1=*) n1=$((n1 + 1)) ;;
	*) other=$((other + 1)) ;;
	esac
done <out2
[ "$n1" -eq 1 ] || pc_fail "the name given with a definition was shown $n1 times"
[ "$other" -eq 0 ] || pc_fail "$other unexpected lines with a definition and a name"
pc_pass

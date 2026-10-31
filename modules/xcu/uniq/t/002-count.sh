# uniq -c puts the number of occurrences before each line, a blank away from the line.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.uniq.002
# MUTANT    uniq.c_zero
# MUTANT    uniq.c_one
# MUTANT    uniq.c_mod10
# MUTANT    uniq.c_global
# MUTANT    uniq.c_after
# MUTANT    uniq.c_nosep
# MUTANT    uniq.c_tab
# MUTANT    uniq.c_only_count
# MUTANT    uniq.c_drop_single
# MUTANT    uniq.no_newline
# MUTANT    uniq.status1
# VARIANT   uniq.count_pad4
# VARIANT   uniq.count_nopad
# VARIANT   uniq.count_pad12
. "$PC_CORE/pc.sh"
pc_begin XCU.uniq.002
LC_ALL=POSIX
export LC_ALL
NL='
'
T=$(printf '\t')
awk 'BEGIN { print "a"; print "a"; print "b"; print "a"; print "c"; print "c"; print "c"; for (i = 0; i < 12; i++) print "d"; print "e"; print "c" }' >in || pc_fail "cannot create input"
pc_test
uniq -c in >out 2>/dev/null </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "uniq -c gave status $rc"
norm=
while IFS= read -r l; do
	while :; do
		case $l in
		" "*|"$T"*) l=${l#?} ;;
		*) break ;;
		esac
	done
	norm=$norm$l$NL
done <out
[ "$norm" = "2 a${NL}1 b${NL}1 a${NL}3 c${NL}12 d${NL}1 e${NL}1 c${NL}" ] || pc_fail "-c output is not count, blank, line for each run"
pc_pass

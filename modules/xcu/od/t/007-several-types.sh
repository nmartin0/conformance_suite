# With several -t types od writes, for each block of input, one line per type in the order given.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.od.007
# MUTANT    od.t_reversed
# MUTANT    od.t_opts_reversed
# MUTANT    od.t_string_reversed
# MUTANT    od.t_first_only
# MUTANT    od.t_last_only
# MUTANT    od.t_string_first_only
# MUTANT    od.t_grouped
# VARIANT   od.gap3
# VARIANT   od.trail_blank
# VARIANT   od.an_empty
. "$PC_CORE/pc.sh"
pc_begin XCU.od.007
LC_ALL=POSIX
export LC_ALL
NL='
'
printf 'ab' >f1 || pc_fail "cannot create input"
pc_test
od -A n -v -t x1 -t c f1 >out 2>/dev/null </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "od -t x1 -t c gave status $rc"
n=0
while IFS= read -r line; do
	set -- $line
	[ $# -gt 0 ] || continue
	n=$((n + 1))
	case $n in
	1) [ "$*" = "61 62" ] || pc_fail "the first line is not the x1 form" ;;
	2) [ "$*" = "a b" ] || pc_fail "the second line is not the c form" ;;
	esac
done <out
[ "$n" -eq 2 ] || pc_fail "two types for one block did not give two lines"
od -A n -v -t c -t x1 f1 >out 2>/dev/null </dev/null
n=0
while IFS= read -r line; do
	set -- $line
	[ $# -gt 0 ] || continue
	n=$((n + 1))
	case $n in
	1) [ "$*" = "a b" ] || pc_fail "with the order reversed the first line is not the c form" ;;
	2) [ "$*" = "61 62" ] || pc_fail "with the order reversed the second line is not the x1 form" ;;
	esac
done <out
[ "$n" -eq 2 ] || pc_fail "reversed types did not give two lines"
od -A n -v -t x1c f1 >out 2>/dev/null </dev/null
n=0
while IFS= read -r line; do
	set -- $line
	[ $# -gt 0 ] || continue
	n=$((n + 1))
done <out
[ "$n" -eq 2 ] || pc_fail "-t x1c did not give two lines"
od -A n -v -t x1c f1 >out 2>/dev/null </dev/null
n=0
while IFS= read -r line; do
	set -- $line
	[ $# -gt 0 ] || continue
	n=$((n + 1))
	case $n in
	1) [ "$*" = "61 62" ] || pc_fail "-t x1c: the first line is not the x1 form" ;;
	2) [ "$*" = "a b" ] || pc_fail "-t x1c: the second line is not the c form" ;;
	esac
done <out
od -A n -v -t cx1 f1 >out 2>/dev/null </dev/null
n=0
while IFS= read -r line; do
	set -- $line
	[ $# -gt 0 ] || continue
	n=$((n + 1))
	case $n in
	1) [ "$*" = "a b" ] || pc_fail "-t cx1: the first line is not the c form" ;;
	2) [ "$*" = "61 62" ] || pc_fail "-t cx1: the second line is not the x1 form" ;;
	esac
done <out
# three types, one string and one option, in the order given
od -A n -v -t o1x1 -t c f1 >out 2>/dev/null </dev/null
n=0
while IFS= read -r line; do
	set -- $line
	[ $# -gt 0 ] || continue
	n=$((n + 1))
	case $n in
	1) [ "$*" = "141 142" ] || pc_fail "three types: the first line is not the o1 form" ;;
	2) [ "$*" = "61 62" ] || pc_fail "three types: the second line is not the x1 form" ;;
	3) [ "$*" = "a b" ] || pc_fail "three types: the third line is not the c form" ;;
	esac
done <out
[ "$n" -eq 3 ] || pc_fail "three types did not give three lines"
# several blocks: the lines of the types alternate block by block
awk 'BEGIN { for (i = 0; i < 100; i++) printf "%c", 97 + i % 26 }' >f100 || pc_fail "cannot create input"
od -A n -v -t x1 -t c f100 >out 2>/dev/null </dev/null
n=0
hexn=0
chn=0
want=x
while IFS= read -r line; do
	set -- $line
	[ $# -gt 0 ] || continue
	n=$((n + 1))
	case $1 in
	??) kind=x; hexn=$((hexn + $#)) ;;
	?) kind=c; chn=$((chn + $#)) ;;
	*) pc_fail "an unexpected field in a two-type listing" ;;
	esac
	[ "$kind" = "$want" ] || pc_fail "the lines of the types do not alternate block by block"
	if [ $want = x ]; then want=c; else want=x; fi
done <out
[ "$hexn" -eq 100 ] && [ "$chn" -eq 100 ] || pc_fail "two types did not each give all 100 bytes"
pc_pass

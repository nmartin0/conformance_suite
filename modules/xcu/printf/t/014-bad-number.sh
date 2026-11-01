# A non-numeric argument is diagnosed and the rest is processed.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.printf.014
# MUTANT    printf.bad_no_diag
# MUTANT    printf.bad_diag_stdout
# MUTANT    printf.bad_status0
# MUTANT    printf.bad_stops
# MUTANT    printf.bad_status_last
# MUTANT    printf.bad_partial_dropped
# MUTANT    printf.sh_bad_no_diag
# MUTANT    printf.exec_bad_no_diag
# MUTANT    printf.sh_bad_status0
# MUTANT    printf.exec_bad_status0
# MUTANT    printf.sh_bad_stops
# MUTANT    printf.exec_bad_stops
# VARIANT   printf.nofloat
# VARIANT   printf.status2
# VARIANT   printf.status255
. "$PC_CORE/pc.sh"
pc_begin XCU.printf.014
nl='
'
bsl='\'
tab=$(echo a | tr a '\011')
bel=$(echo a | tr a '\007')
bsp=$(echo a | tr a '\010')
ff=$(echo a | tr a '\014')
cr=$(echo a | tr a '\015')
vt=$(echo a | tr a '\013')
# pf WANT FORMAT [ARG...]: printf must write exactly WANT, called by the
# shell and by a direct exec, and exit zero.
pf() {
	want=$1
	shift
	got=$(printf "$@" </dev/null 2>/dev/null; echo "x$?")
	[ "$got" = "${want}x0" ] || pc_fail "printf $* did not write what was wanted, or did not exit zero"
	got=$( (exec printf "$@") </dev/null 2>/dev/null; echo "x$?")
	[ "$got" = "${want}x0" ] || pc_fail "exec printf $* did not write what was wanted, or did not exit zero"
}
pc_test
# chk WHO CONV ARG WANT: with an argument that is not entirely a number, a
# diagnostic goes to standard error, the status is not zero, the value
# accumulated so far is written, and the next argument is still processed.
chk() {
	who=$1 conv=$2 arg=$3 want=$4
	if [ "$who" = sh ]; then
		printf "%$conv\\n" "$arg" 7 >out 2>err </dev/null
		rc=$?
	else
		(exec printf "%$conv\\n" "$arg" 7) >out 2>err </dev/null
		rc=$?
	fi
	[ "$rc" -ne 0 ] || pc_fail "$who printf exited zero for the argument $arg to %$conv"
	[ -s err ] || pc_fail "$who printf wrote no diagnostic for the argument $arg to %$conv"
	[ -f out ] || pc_fail "$who printf: no output file"
	first= last=
	n=0
	while IFS= read -r line; do
		n=$((n + 1))
		[ $n -eq 1 ] && first=$line
		last=$line
	done <out
	[ "$last" = 7 ] || pc_fail "$who printf did not process the argument after the bad one $arg to %$conv; last line is '$last'"
	[ "$n" -eq 2 ] || pc_fail "$who printf wrote $n lines for %$conv with $arg and 7, not 2"
	[ "$first" = "$want" ] || pc_fail "$who printf wrote '$first' for %$conv of $arg, not the value accumulated '$want'"
}
for who in sh exec; do
	for conv in d i o u x X; do
		chk $who $conv abc 0
	done
	chk $who d 12abc 12
	chk $who d 3.5 3
	chk $who x 12zz c
	chk $who o 12zz 14
	chk $who u 12zz 12
done
pc_pass

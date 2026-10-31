# head -n number writes that many lines; a number larger than the file copies it whole without error.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.head.002
# MUTANT    head.n_less
# MUTANT    head.n_more
# MUTANT    head.n_ignored
# MUTANT    head.n_as_bytes
# MUTANT    head.n_cap10
# MUTANT    head.short_status1
# MUTANT    head.short_nothing
# MUTANT    head.long_line_cut
# MUTANT    head.header_single
. "$PC_CORE/pc.sh"
pc_begin XCU.head.002
LC_ALL=POSIX
export LC_ALL
NL='
'
printf '%s\n' 1 2 3 4 5 6 7 8 9 10 11 12 >f12 || pc_fail "cannot create input"
# A line far longer than any line-length limit (the text says the length is not restricted).
# Built in pieces, so no single command-line argument is very large.
c=xxxxxxxxxxxxxxxx
i=0
while [ $i -lt 6 ]; do c=$c$c; i=$((i + 1)); done
: >big; : >bigexp
i=0
while [ $i -lt 128 ]; do
	printf '%s' "$c" >>big && printf '%s' "$c" >>bigexp || pc_fail "cannot create input"
	i=$((i + 1))
done
printf '\nsecond\nthird\n' >>big || pc_fail "cannot create input"
printf '\nsecond\n' >>bigexp || pc_fail "cannot create input"
pc_test
got=$(head -n 3 f12 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "1${NL}2${NL}3${NL}rc=0" ] || pc_fail "-n 3 did not write exactly three lines"
got=$(head -n 1 f12 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "1${NL}rc=0" ] || pc_fail "-n 1 did not write exactly one line"
got=$(head -n 12 f12 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "1${NL}2${NL}3${NL}4${NL}5${NL}6${NL}7${NL}8${NL}9${NL}10${NL}11${NL}12${NL}rc=0" ] || pc_fail "-n 12 did not write the whole file"
got=$(head -n 50 f12 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "1${NL}2${NL}3${NL}4${NL}5${NL}6${NL}7${NL}8${NL}9${NL}10${NL}11${NL}12${NL}rc=0" ] || pc_fail "-n 50 on a 12-line file was not the whole file with status 0"
head -n 2 big >bigout 2>/dev/null </dev/null || pc_fail "-n 2 failed on a file with a very long line"
cmp -s bigout bigexp || pc_fail "a very long line was not copied intact"
pc_pass

# Options followed by several operands: every operand is used.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XBD.12.2-utility-syntax-guidelines.003
. "$PC_CORE/pc.sh"
pc_begin XBD.12.2-utility-syntax-guidelines.003
LC_ALL=POSIX
export LC_ALL
NL='
'
printf 'alpha\nbeta\n' >f1 || pc_unresolved "cannot create f1"
printf 'gamma\ndelta\n' >f2 || pc_unresolved "cannot create f2"
printf 'extra\n' >f3 || pc_unresolved "cannot create f3"
pc_test
chk() { # <what> <got> <expected>
	[ "$2" = "$3" ] || pc_fail "$1 gave '$2', expected '$3'"
}
chk "cat f1 f2 f3" "$(cat f1 f2 f3 2>/dev/null </dev/null)" "alpha${NL}beta${NL}gamma${NL}delta${NL}extra"
chk "grep -n -e et -e mm f1 f2" "$(grep -n -e et -e mm f1 f2 2>/dev/null </dev/null)" "f1:2:beta${NL}f2:1:gamma"
chk "sed -n p f1 f2" "$(sed -n p f1 f2 2>/dev/null </dev/null)" "alpha${NL}beta${NL}gamma${NL}delta"
chk "ls f3 f1 f2" "$(ls f3 f1 f2 2>/dev/null </dev/null)" "f1${NL}f2${NL}f3"
c1= c2=
wc -l f1 f2 2>/dev/null </dev/null >wc.out
while read -r cnt nm; do
	case $nm in
	f1) c1=$cnt ;;
	f2) c2=$cnt ;;
	esac
done <wc.out
[ "$c1" = 2 ] && [ "$c2" = 2 ] || pc_fail "wc -l f1 f2 gave counts '$c1' and '$c2' for f1 and f2, expected 2 and 2"
seq=
head -n 1 f1 f2 2>/dev/null </dev/null >head.out
while IFS= read -r l; do
	[ -n "$l" ] && seq="$seq|$l"
done <head.out
[ "$seq" = "|==> f1 <==|alpha|==> f2 <==|gamma" ] || pc_fail "head -n 1 f1 f2 gave the non-empty lines '$seq'"
mkdir d1 d2 d3 2>/dev/null </dev/null || pc_fail "mkdir d1 d2 d3 failed"
[ -d d1 ] && [ -d d2 ] && [ -d d3 ] || pc_fail "mkdir d1 d2 d3 did not create all three directories"
rm f1 f2 f3 2>/dev/null </dev/null || pc_fail "rm f1 f2 f3 failed"
[ ! -e f1 ] && [ ! -e f2 ] && [ ! -e f3 ] || pc_fail "rm f1 f2 f3 left one of them behind"
pc_pass

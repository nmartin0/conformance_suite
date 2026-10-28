# assorted ex commands
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ex.062
. "$PC_CORE/pc.sh"
pc_begin XCU.ex.062
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
n=0
printf 'r1\nr2\n' >rf || pc_unresolved "cannot write rf"
: >emptyf || pc_unresolved "cannot write emptyf"
# exchk LABEL INITIAL SCRIPT EXPECTED : the script is followed by w and q
exchk() {
	n=$((n + 1))
	printf '%b' "$2" >fc$n || pc_unresolved "cannot write the input"
	printf 'set noap\n%s\nw\nq\n' "$3" >sc$n
	printf '%b' "$4" >ec$n
	ex -s fc$n <sc$n >oc$n 2>rc$n
	r=$?
	[ "$r" -eq 0 ] || pc_fail "$1: exit status $r: $(head -c 200 rc$n)"
	cmp -s fc$n ec$n || pc_fail "$1: the file holds '$(tr '\n' '|' <fc$n)', wanted '$(tr '\n' '|' <ec$n)'"
}
# exout LABEL INITIAL SCRIPT EXPECTED-OUTPUT : the script is followed by q!
exout() {
	n=$((n + 1))
	printf '%b' "$2" >fo$n || pc_unresolved "cannot write the input"
	printf 'set noap\n%s\nq!\n' "$3" >so$n
	printf '%b' "$4" >eo$n
	ex -s fo$n <so$n >oo$n 2>ro$n
	r=$?
	[ "$r" -eq 0 ] || pc_fail "$1: exit status $r: $(head -c 200 ro$n)"
	cmp -s oo$n eo$n || pc_fail "$1: the output is '$(tr '\n' '|' <oo$n)', wanted '$(tr '\n' '|' <eo$n)'"
}
# excl LABEL INITIAL SCRIPT LINE : the line number of the current line after the script
excl() {
	n=$((n + 1))
	printf '%b' "$2" >fl$n || pc_unresolved "cannot write the input"
	printf 'set noap\n%s\n.=\nq!\n' "$3" >sl$n
	ex -s fl$n <sl$n >ol$n 2>rl$n
	r=$?
	[ "$r" -eq 0 ] || pc_fail "$1: exit status $r: $(head -c 200 rl$n)"
	got=$(tail -n 1 ol$n)
	[ "$got" = "$4" ] || pc_fail "$1: the current line is '$got', wanted '$4'"
}
# exfail LABEL INITIAL SCRIPT : the script must fail with a non-zero status
exfail() {
	n=$((n + 1))
	printf '%b' "$2" >ff$n || pc_unresolved "cannot write the input"
	printf 'set noap\n%s\nw\nq\n' "$3" >sf$n
	ex -s ff$n <sf$n >of$n 2>rf$n
	r=$?
	[ "$r" -ne 0 ] || pc_fail "$1: the error was not reported by the exit status"
}

mkdir hd || pc_unresolved "mkdir"
pc_test
exfail 'address zero' 'a\nb\nc\nd\ne\n' '0d'
exchk 'default range is the current line' 'a\nb\nc\nd\ne\n' '3a
.
d' 'a\nb\nd\ne\n'
exfail 'first address after the second' 'a\nb\nc\nd\ne\n' '3,2d'
exchk 'insert at line zero' 'a\nb\nc\nd\ne\n' '0i
X
.' 'X\na\nb\nc\nd\ne\n'
exchk 'insert at line one' 'a\nb\nc\nd\ne\n' '1i
X
.' 'X\na\nb\nc\nd\ne\n'
exchk 'ch is change' 'a\nb\nc\nd\ne\n' '2ch
X
.' 'a\nX\nc\nd\ne\n'
exout 'pr is print' 'a\nb\nc\nd\ne\n' '2pr' 'b
'
exchk 're is read' 'a\nb\nc\nd\ne\n' '1re rf' 'a\nr1\nr2\nb\nc\nd\ne\n'
exchk 'ma is mark' 'a\nb\nc\nd\ne\n' '2ma b
'\''bd' 'a\nc\nd\ne\n'
exchk 'un is undo' 'a\nb\nc\nd\ne\n' '2d
un' 'a\nb\nc\nd\ne\n'
exchk 'a vertical line ends a map command' 'a\nb\nc\nd\ne\n' 'map x y|2d' 'a\nc\nd\ne\n'
exfail 'global inside global' 'a\nb\n' 'g/a/g/a/p'
exfail 'undo inside global' 'a\nb\n' 'g/a/u'
exout 'ignorecase applies to the remembered expression' 'x\nB\nB\nb\n' 'set ic
/b/
set noic
1;//=' 'B
4
'
exfail 'unabbreviate of an unknown name' 'a\nb\nc\nd\ne\n' 'una nosuch'
exfail 'unmap of an unknown name' 'a\nb\nc\nd\ne\n' 'unmap x'
exfail 'unmap! of an unknown name' 'a\nb\nc\nd\ne\n' 'unmap! x'
exout '= defaults to the last line' 'a\nb\nc\nd\ne\n' '=' '5
'

# cd with HOME, and with a modified buffer
printf 'a\nb\nc\n' >f
printf 'cd\nw outcd\nq\n' >s.cd
OHOME=$HOME
HOME=$PWD/hd
export HOME
ex -s f <s.cd >o.cd 2>e.cd || pc_fail "cd: exit status $?"
HOME=$OHOME
export HOME
[ -f hd/outcd ] || pc_fail "cd with no argument did not go to HOME"
printf 'a\nb\nc\n' >f
printf '1d\ncd hd\nq!\n' >s.cdm
ex -s f <s.cdm >o.cdm 2>e.cdm
[ "$?" -ne 0 ] || pc_fail "cd from a modified buffer with a relative pathname was accepted"
# shifting
printf 'a\nb\nc\nd\ne\n' >f
printf 'set noap\n2>\n$pu\nw\nq\n' >s.sh
ex -s f <s.sh >o.sh 2>e.sh || pc_fail "shift: exit status $?"
[ "$(tail -n 1 f)" = b ] || pc_fail "the shifted line was not saved in the unnamed buffer"
[ "$(wc -l <f | tr -d ' ')" = 6 ] || pc_fail "the file has the wrong number of lines after a shift and a put"
printf 'a\nb\nc\n' >f
printf '2>\nq!\n' >s.ap
ex -s f <s.ap >o.ap 2>e.ap || pc_fail "autoprint after a shift: exit status $?"
[ "$(sed 's/^[ 	]*//' o.ap)" = b ] || pc_fail "autoprint after a shift wrote '$(cat o.ap)'"
printf 'a\n\nb\n' >f
printf 'set noap\n1,3>\nw\nq\n' >s.em
ex -s f <s.em >o.em 2>e.em || pc_fail "shifting an empty line: exit status $?"
[ "$(grep -c '^$' f)" = 1 ] || pc_fail "an empty line was changed by >"
# set queries
printf 'a\n' >f
printf 'set sw=3\nset sw?\nq!\n' >s.q1
ex -s f <s.q1 >o.q1 2>e.q1 || pc_fail "set sw?: exit status $?"
grep 3 o.q1 >/dev/null || pc_fail "set sw? did not write the value: '$(cat o.q1)'"
printf 'set sw=3\nset sw ?\nq!\n' >s.q2
ex -s f <s.q2 >o.q2 2>e.q2 || pc_fail "set sw ?: exit status $?"
grep 3 o.q2 >/dev/null || pc_fail "set sw ? did not write the value"
printf 'set sw=3\nset sw\nq!\n' >s.q3
ex -s f <s.q3 >o.q3 2>e.q3 || pc_fail "set sw: exit status $?"
grep 3 o.q3 >/dev/null || pc_fail "set sw did not write the value"
# quit with more files to edit
printf 'a\n' >g1
printf 'a\n' >g2
printf 'q\n' >s.q
ex -s g1 g2 <s.q >o.q 2>e.q
[ "$?" -ne 0 ] || pc_fail "q with another file still to edit was accepted"
# readonly follows write permission
printf 'a\nb\n' >f
chmod 444 f || pc_unresolved "chmod"
printf 'd\nw\nq\n' >s.ro
ex -s f <s.ro >o.ro 2>e.ro
r=$?
if [ -w f ]; then
	[ "$r" -eq 0 ] || pc_fail "writing a file the process may write failed: status $r"
else
	[ "$r" -ne 0 ] || pc_fail "writing a file the process may not write was accepted"
	[ "$(wc -l <f | tr -d ' ')" = 2 ] || pc_fail "the read-only file was changed"
fi
pc_pass

# current line after commands
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ex.020
. "$PC_CORE/pc.sh"
pc_begin XCU.ex.020
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
pc_test
excl 'append' 'a\nb\nc\nd\ne\n' '2a
X
Y
.' '4'
excl 'append at 0' 'a\nb\nc\nd\ne\n' '0a
X
.' '1'
excl 'append nothing' 'a\nb\nc\nd\ne\n' '3a
.' '3'
excl 'insert' 'a\nb\nc\nd\ne\n' '3i
X
Y
.' '4'
excl 'insert nothing' 'a\nb\nc\nd\ne\n' '3i
.' '2'
excl 'insert nothing at 1' 'a\nb\nc\nd\ne\n' '1i
.' '1'
excl 'change' 'a\nb\nc\nd\ne\n' '2,3c
X
.' '2'
excl 'change nothing' 'a\nb\nc\nd\ne\n' '2,3c
.' '1'
excl 'change nothing at 1' 'a\nb\nc\nd\ne\n' '1,2c
.' '1'
excl 'delete middle' 'a\nb\nc\nd\ne\n' '2d' '2'
excl 'delete last' 'a\nb\nc\nd\ne\n' '5d' '4'
excl 'delete all' 'a\nb\nc\nd\ne\n' '1,5d' '0'
excl 'delete range' 'a\nb\nc\nd\ne\n' '2,3d' '2'
excl 'copy' 'a\nb\nc\nd\ne\n' '1,2t4' '6'
excl 'copy to 0' 'a\nb\nc\nd\ne\n' '1,2t0' '2'
excl 'move' 'a\nb\nc\nd\ne\n' '1,2m4' '4'
excl 'move to 0' 'a\nb\nc\nd\ne\n' '4,5m0' '2'
excl 'join range' 'a\nb\nc\nd\ne\n' '2,4j' '2'
excl 'join one' 'a\nb\nc\nd\ne\n' '4j' '4'
excl 'substitute range' 'a\nb\nc\nd\ne\n' '2,4s/./X/' '4'
excl 'substitute some' 'a\nb\nc\nd\ne\n' '1,5s/[bd]/X/' '4'
excl 'repeat' 'a\nb\nc\nd\ne\n' '2s/./X/
4&' '4'
excl 'global' 'a\nb\nc\nd\ne\n' 'g/[bd]/s/$/!/' '4'
excl 'read' 'a\nb\nc\nd\ne\n' '3r rf' '5'
excl 'read at 0' 'a\nb\nc\nd\ne\n' '0r rf' '2'
excl 'put' 'a\nb\nc\nd\ne\n' '1ya
3pu' '4'
excl 'put range' 'a\nb\nc\nd\ne\n' '2,3ya
1pu' '3'
excl 'shift' 'a\nb\nc\nd\ne\n' '2,3>
2,3<' '3'
excl 'print' 'a\nb\nc\nd\ne\n' '2,4p' '4'
excl 'equal' 'a\nb\nc\nd\ne\n' '2=' '5'
excl 'mark' 'a\nb\nc\nd\ne\n' '2ka' '5'
excl 'yank' 'a\nb\nc\nd\ne\n' '2,3ya' '5'
excl 'mark command' 'a\nb\nc\nd\ne\n' '2mark b' '5'
excl 'file' 'a\nb\nc\nd\ne\n' 'f' '5'
excl 'args' 'a\nb\nc\nd\ne\n' 'ar' '5'
excl 'set' 'a\nb\nc\nd\ne\n' 'set list' '5'
excl 'map' 'a\nb\nc\nd\ne\n' 'map x y' '5'
excl 'unmap' 'a\nb\nc\nd\ne\n' 'map x y
unmap x' '5'
excl 'abbreviate' 'a\nb\nc\nd\ne\n' 'ab q rr' '5'
excl 'unabbreviate' 'a\nb\nc\nd\ne\n' 'ab q rr
una q' '5'
excl 'version' 'a\nb\nc\nd\ne\n' 've' '5'
excl 'cd' 'a\nb\nc\nd\ne\n' 'cd .' '5'
excl 'write' 'a\nb\nc\nd\ne\n' 'w wout' '5'
excl 'undo delete' 'a\nb\nc\nd\ne\n' '2,3d
u' '2'
excl 'undo delete last' 'a\nb\nc\nd\ne\n' '$d
u' '5'
excl 'undo delete first' 'a\nb\nc\nd\ne\n' '1d
u' '1'
excl 'edit discarding' 'a\nb\nc\nd\ne\n' '2d
e!' '5'
excl 'global without commands' 'a\nb\nc\nd\ne\n' 'g/[bd]/' '4'
excl 'read of an empty file' 'a\nb\nc\nd\ne\n' 'r emptyf' '5'
excl 'undo of an append' 'a\nb\nc\nd\ne\n' '2a
X
.
u' '2'
excl 'undo of an append at the top' 'a\nb\nc\nd\ne\n' '0a
X
.
u' '1'
excl 'empty buffer' '' 'set list' '0'
excl 'undo back to an empty buffer' '' 'a
X
.
u' '0'
pc_pass

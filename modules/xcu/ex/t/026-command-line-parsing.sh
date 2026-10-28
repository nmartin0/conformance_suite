# command line parsing
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ex.026
. "$PC_CORE/pc.sh"
pc_begin XCU.ex.026
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
exchk 'leading colons and blanks' 'a\nb\nc\nd\ne\n' '::  2d' 'a\nc\nd\ne\n'
exchk 'comment line' 'a\nb\nc\nd\ne\n' '" remark 2d
2d' 'a\nc\nd\ne\n'
exchk 'trailing comment' 'a\nb\nc\nd\ne\n' '2d "tail' 'a\nc\nd\ne\n'
exchk 'vertical line' 'a\nb\nc\nd\ne\n' '1d|1d' 'c\nd\ne\n'
exchk 'vertical line with blanks' 'a\nb\nc\nd\ne\n' '1d | 1d' 'c\nd\ne\n'
exchk 'full command name' 'a\nb\nc\nd\ne\n' '2delete' 'a\nc\nd\ne\n'
exchk 'del is delete' 'a\nb\nc\nd\ne\n' '2del' 'a\nc\nd\ne\n'
exchk 'k then a mark name' 'a\nb\nc\nd\ne\n' '2kb
'\''bd' 'a\nc\nd\ne\n'
exchk 's followed by options' 'x x\nx x\nx x\n' '1s/x/y/
2sg' 'y x\ny y\nx x\n'
exchk 'count without a blank' 'a\nb\nc\nd\ne\n' '1d2' 'c\nd\ne\n'
exout 'address alone prints the line' 'a\nb\nc\nd\ne\n' '2' 'b
'
exout 'empty line prints the next line' 'a\nb\nc\nd\ne\n' '1

.=' 'a
b
2
'
exfail 'empty line past the last line' 'a\nb\nc\nd\ne\n' '$
'
exout 'flags are remembered by the implied print' 'a\nb\nc\nd\ne\n' '2l
3
4p
5' 'b$
c$
d
e
'
exout 'two vertical lines give two implied commands' 'a\nb\nc\nd\ne\n' '1
||' 'a
b
c
'
pc_pass

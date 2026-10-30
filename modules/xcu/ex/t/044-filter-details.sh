# filter details
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ex.044
. "$PC_CORE/pc.sh"
pc_begin XCU.ex.044
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
	[ "$r" -eq 0 ] || pc_fail "$1: exit status $r: $(dd bs=200 count=1 <rc$n 2>/dev/null)"
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
	[ "$r" -eq 0 ] || pc_fail "$1: exit status $r: $(dd bs=200 count=1 <ro$n 2>/dev/null)"
	cmp -s oo$n eo$n || pc_fail "$1: the output is '$(tr '\n' '|' <oo$n)', wanted '$(tr '\n' '|' <eo$n)'"
}
# excl LABEL INITIAL SCRIPT LINE : the line number of the current line after the script
excl() {
	n=$((n + 1))
	printf '%b' "$2" >fl$n || pc_unresolved "cannot write the input"
	printf 'set noap\n%s\n.=\nq!\n' "$3" >sl$n
	ex -s fl$n <sl$n >ol$n 2>rl$n
	r=$?
	[ "$r" -eq 0 ] || pc_fail "$1: exit status $r: $(dd bs=200 count=1 <rl$n 2>/dev/null)"
	got=$(tail -n 1 ol$n | tr -d '!')
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
printf 'p\r\nq\r\n' >crlf || pc_unresolved "write"
pc_test
exchk 'filter lines' 'a\nb\nc\nd\ne\n' '2,3!sort -r' 'a\nc\nb\nd\ne\n'
exchk 'the unnamed buffer holds the replaced lines' 'a\nb\nc\nd\ne\n' '2,3!sort -r
$pu' 'a\nc\nb\nd\ne\nb\nc\n'
exchk 'output without a final newline' 'a\nb\nc\nd\ne\n' '2!printf x' 'a\nx\nc\nd\ne\n'
exchk 'carriage return newline pairs' 'a\nb\nc\nd\ne\n' '2!cat crlf' 'a\np\nq\nc\nd\ne\n'
excl 'current line after a filter' 'a\nb\nc\nd\ne\n' '2,3!sort -r' '3'
excl 'current line when the output is empty' 'a\nb\nc\nd\ne\n' '2,3!true' '1'
excl 'current line when the first line is replaced by nothing' 'a\nb\nc\nd\ne\n' '1,2!true' '1'
excl 'current line when everything is replaced by nothing' 'a\nb\nc\nd\ne\n' '1,5!true' '0'
exchk 'a command without lines leaves the buffer' 'a\nb\nc\nd\ne\n' '!true' 'a\nb\nc\nd\ne\n'
pc_pass

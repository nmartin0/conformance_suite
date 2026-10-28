# read details
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ex.042
. "$PC_CORE/pc.sh"
pc_begin XCU.ex.042
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
printf 'x' >nonl || pc_unresolved "write"
printf 'p\r\nq\r\n' >crlf || pc_unresolved "write"
printf 'bang\n' >'!x' || pc_unresolved "write"
pc_test
exchk 'read with no file name' 'a\nb\n' '1r' 'a\na\nb\nb\n'
exchk 'incomplete last line' 'a\nb\n' '1r nonl' 'a\nx\nb\n'
exchk 'command output without a final newline' 'a\nb\n' '1r !printf x' 'a\nx\nb\n'
exchk 'carriage return newline pairs' 'a\nb\n' '1r !cat crlf' 'a\np\nq\nb\n'
exfail 'missing file' 'a\nb\n' '1r nosuchfile'
exchk 'backslash before the bang' 'a\n' 'r \!x' 'a\nbang\n'
printf 'a\n' >fw
printf 'w \!w1\nq\n' >sw
ex -s fw <sw >ow 2>ew || pc_fail "w with an escaped bang: exit status $?"
[ -f '!w1' ] || pc_fail "w with a backslash before the bang did not write a file"
pc_pass

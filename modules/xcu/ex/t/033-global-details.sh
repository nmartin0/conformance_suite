# global command details
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ex.033
. "$PC_CORE/pc.sh"
pc_begin XCU.ex.033
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
exchk 'g! is v' 'ab\ncd\nab\nef\n' 'g!/ab/d' 'ab\nab\n'
exchk 'v' 'ab\ncd\nab\nef\n' 'v/ab/d' 'ab\nab\n'
exchk 'default range is the whole buffer' 'ab\ncd\nab\nef\n' 'g/ab/d' 'cd\nef\n'
exchk 'null pattern is the last expression' 'ab\ncd\nab\nef\n' '/ef/
g//d' 'ab\ncd\nab\n'
exchk 'other delimiter' 'ab\ncd\nab\nef\n' 'g#ab#d' 'cd\nef\n'
exout 'delimiter inside a bracket expression' 'a%\nb\n%c\n' 'g%[%]%p' 'a%
%c
'
exchk 'commands joined by vertical lines' 'ab\ncd\nab\nef\n' 'g/ab/s/a/X/|s/b/Y/' 'XY\ncd\nXY\nef\n'
exchk 'commands on separate lines' 'ab\ncd\nab\nef\n' 'g/ab/s/a/X/\
s/b/Y/' 'XY\ncd\nXY\nef\n'
exout 'no command prints the lines' 'ab\ncd\nab\nef\n' 'g/ab/' 'ab
ab
'
exchk 'no command runs for a deleted marked line' 'x\ny\nz\nw\n' 'g/[xyz]/.+1d' 'x\nz\n'
exfail 'an error in the commands' 'ab\ncd\nab\nef\n' 'g/ab/s/zzz/x/'
pc_pass

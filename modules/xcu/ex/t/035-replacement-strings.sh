# replacement strings
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ex.035
. "$PC_CORE/pc.sh"
pc_begin XCU.ex.035
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
exchk 'ampersand is the match' 'cat\n' 's/cat/[&]/' '[cat]\n'
exchk 'escaped ampersand is literal' 'cat\n' 's/cat/\&/' '&\n'
exchk 'back references' 'ab cd\n' 's/\(a\)\(b\)/\2\1/' 'ba cd\n'
exchk 'a group that did not match is empty' 'ab\n' 's/\(x\)*b/[\1]/' 'a[]\n'
exchk 'tilde is the previous replacement' 'cat\ndog\ncat\n' '1s/cat/bat/
3s/cat/~/' 'bat\ndog\nbat\n'
exchk 'escaped tilde is literal' 'cat\n' 's/cat/\~/' '~\n'
exchk 'percent is the previous replacement' 'cat\ndog\ncat\n' '1s/cat/Q/
3s/cat/%/' 'Q\ndog\nQ\n'
exchk '\u makes one capital' 'The cat sat on the mat.\n' 's/\<.at\>/\u&/g' 'The Cat Sat on the Mat.\n'
exchk '\U to \e' 'The cat sat on the mat.\n' 's/\<.at\>/\u&/g
s/S\(.*\)M/S\U\1\eM/' 'The Cat SAT ON THE Mat.\n'
exchk '\L lowers all' 'ABC DEF\n' 's/.*/\L&/' 'abc def\n'
exchk '\l lowers one' 'ABC\n' 's/.*/\l&/' 'aBC\n'
exchk 'backslash before another character' 'ab\n' 's/a/\x/' 'xb\n'
pc_pass

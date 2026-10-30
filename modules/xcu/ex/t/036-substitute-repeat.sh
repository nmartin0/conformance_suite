# repeating substitutions
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ex.036
. "$PC_CORE/pc.sh"
pc_begin XCU.ex.036
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
exchk 'empty command repeats' 'aa\naa\n' '1s/a/b/
2s//' 'ba\nba\n'
exchk 'empty pattern is the last expression' 'aa\naa\n' '1s/a/b/
2s//X/' 'ba\nXa\n'
exchk 'ampersand repeats' 'aa\naa\n' '1s/a/b/
2&' 'ba\nba\n'
exchk 'ampersand with g' 'aa\naa\n' '1s/a/b/
2&g' 'ba\nbb\n'
exchk 'tilde command uses the last expression' 'ab\ncd\nab\n' '1s/a/X/
/c/
~' 'Xb\nXd\nab\n'
exchk 'empty replacement and no final delimiter' 'abc\nabc\n' '1s/b//
2s/b' 'ac\nac\n'
exchk 'no final delimiter after the replacement' 'abc\n' 's/b/X' 'aXc\n'
exchk 'success on some lines is not an error' 'a\nc\n' '1,2s/a/b/' 'b\nc\n'
exfail 'no line matched' 'a\nc\n' '1,2s/zzz/x/'
exchk 'dash as delimiter' 'a1b22c\n' 's-[0-9]--g' 'abc\n'
exchk 'hash as delimiter' 'a/b\n' 's#/#-#' 'a-b\n'
exchk 'escaped delimiter in the replacement' 'ab\n' 's/a/x\/y/' 'x/yb\n'
exchk 'escaped delimiter in the expression' 'a/b\n' 's/a\/b/X/' 'X\n'
exchk 'tilde in an expression' 'a\nX\n' '1s/a/X/
2s/~/Y/' 'X\nY\n'
exchk 'escaped tilde in an expression' 'a\n~\n' '1s/a/X/
2s/\~/Q/' 'X\nQ\n'
pc_pass

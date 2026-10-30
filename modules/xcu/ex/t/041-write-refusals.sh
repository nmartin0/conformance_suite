# refused writes
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ex.041
. "$PC_CORE/pc.sh"
pc_begin XCU.ex.041
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

five() { printf 'one\ntwo\nthree\nfour\nfive\n'; }
five >orig
printf 'G\n' >g
pc_test
run() {
	five >f
	printf 'G\n' >g
	printf '%s\n' "$2" >s.$1
	ex -s f <s.$1 >o.$1 2>e.$1
	r=$?
}
run part '1,2w
q!'
[ "$r" -ne 0 ] || pc_fail "writing part of the buffer over the current file was accepted"
cmp -s f orig || pc_fail "the file changed after a refused partial write"
run partbang '1,2w!
q!'
[ "$r" -eq 0 ] || pc_fail "w! of part of the buffer: exit status $r"
printf 'one\ntwo\n' >exp
cmp -s f exp || pc_fail "w! of part of the buffer wrote '$(tr '\n' '|' <f)'"
run any 'set writeany
1,2w
q!'
[ "$r" -eq 0 ] || pc_fail "writeany: exit status $r"
cmp -s f exp || pc_fail "writeany did not allow a partial write: $(tr '\n' '|' <f)"
run fchg 'f g
w
q!'
[ "$r" -ne 0 ] || pc_fail "writing to an existing file after f was accepted"
printf 'G\n' >exp
cmp -s g exp || pc_fail "the existing file was overwritten"
run fbang 'f g
w!
1d
w
q!'
[ "$r" -eq 0 ] || pc_fail "w! then w: exit status $r"
printf 'two\nthree\nfour\nfive\n' >exp
cmp -s g exp || pc_fail "a later w after a successful w! failed or wrote the wrong text: $(tr '\n' '|' <g)"
run empty '1,5d
w e1
q!'
[ "$r" -eq 0 ] || pc_fail "writing an empty buffer: exit status $r"
[ -f e1 ] && [ ! -s e1 ] || pc_fail "an empty buffer did not give an empty file"
run other 'w g
q!'
[ "$r" -ne 0 ] || pc_fail "w to another existing file was accepted"
pc_pass

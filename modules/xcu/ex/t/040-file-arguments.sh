# file arguments
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ex.040
. "$PC_CORE/pc.sh"
pc_begin XCU.ex.040
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

mkdir hd || pc_unresolved "mkdir"
printf 'H\n' >hd/hf || pc_unresolved "write"
printf 'Y\n' >y1 || pc_unresolved "write"
: >x1; : >x2
pc_test
run() { # run NAME script-text ; the input file is f, content one line "F1"
	printf 'F1\n' >f || pc_unresolved "cannot write f"
	printf 'G1\n' >g || pc_unresolved "cannot write g"
	printf '%s\n' "$2" >s.$1
	ex -s f <s.$1 >o.$1 2>e.$1
	r=$?
}
run pc 'w %.bak
q'
[ "$r" -eq 0 ] || pc_fail "percent: exit status $r"
cmp -s f.bak f || pc_fail "percent in a file argument was not the current pathname"
run alt 'e g
r #
w
q'
[ "$r" -eq 0 ] || pc_fail "alternate: exit status $r"
printf 'G1\nF1\n' >exp
cmp -s g exp || pc_fail "# did not stand for the alternate pathname: $(tr '\n' '|' <g)"
run ff 'f hname
w
q'
[ "$r" -eq 0 ] || pc_fail "file command: exit status $r"
cmp -s hname f || pc_fail "f did not change the current pathname"
run wr 'w h
r #
w
q'
[ "$r" -eq 0 ] || pc_fail "write then read #: exit status $r"
printf 'F1\nF1\n' >exp
cmp -s f exp || pc_fail "w did not set the alternate pathname: $(tr '\n' '|' <f)"
run un 'r #
q'
[ "$r" -ne 0 ] || pc_fail "# without an alternate pathname was accepted"
run esc 'w a\%b
q'
[ "$r" -eq 0 ] || pc_fail "escaped percent: exit status $r"
[ -f 'a%b' ] || pc_fail "an escaped % was not taken literally"
run blank 'w c\ d
q'
[ "$r" -eq 0 ] || pc_fail "escaped blank: exit status $r"
[ -f 'c d' ] || pc_fail "an escaped blank did not stay in the file name"
run glob 'r y*
w
q'
[ "$r" -eq 0 ] || pc_fail "pattern in a file argument: exit status $r"
printf 'F1\nY\n' >exp
cmp -s f exp || pc_fail "a pattern in a file argument was not expanded: $(tr '\n' '|' <f)"
run nx 'n g
r #
w
q'
[ "$r" -eq 0 ] || pc_fail "n: exit status $r"
printf 'G1\nF1\n' >exp
cmp -s g exp || pc_fail "n did not set the alternate pathname: $(tr '\n' '|' <g)"
run many 'r x*
q'
[ "$r" -ne 0 ] || pc_fail "a pattern matching two files was accepted for one file"
HOME=$PWD/hd
export HOME
run tilde 'r ~/hf
w
q'
[ "$r" -eq 0 ] || pc_fail "tilde: exit status $r"
printf 'F1\nH\n' >exp
cmp -s f exp || pc_fail "~ in a file argument was not expanded: $(tr '\n' '|' <f)"
HDIR=$PWD/hd
export HDIR
run var 'r $HDIR/hf
w
q'
[ "$r" -eq 0 ] || pc_fail "variable: exit status $r"
cmp -s f exp || pc_fail "a variable in a file argument was not expanded: $(tr '\n' '|' <f)"
pc_pass

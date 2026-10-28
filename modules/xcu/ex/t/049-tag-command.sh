# the tag command
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ex.049
. "$PC_CORE/pc.sh"
pc_begin XCU.ex.049
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

command -v ctags >/dev/null 2>&1 || pc_notinuse "the tag feature goes with the ctags utility, which is absent"
printf 'main\n' >f
printf 'alpha\nbeta\ngamma\n' >f2
tab=$(printf '\t')
printf 'tbig%sf2%s99\ntg%sf2%s/^beta/\ntl%sf2%s3\ntnopat%sf2%s/^zzz/\n' "$tab" "$tab" "$tab" "$tab" "$tab" "$tab" "$tab" "$tab" >tagsfile
printf 'other%sf2%s1\n' "$tab" "$tab" >tagsnone
pc_test
run() {
	printf 'main\n' >f
	printf 'alpha\nbeta\ngamma\n' >f2
	printf '%s\n' "$2" >s.$1
	ex -s f <s.$1 >o.$1 2>e.$1
	r=$?
}
run pat 'set tags=tagsfile
tag tg
s/$/!/
w
q'
[ "$r" -eq 0 ] || pc_fail "tag with a pattern: exit status $r: $(head -c 200 e.pat)"
printf 'alpha\nbeta!\ngamma\n' >exp
cmp -s f2 exp || pc_fail "the pattern did not select the line: $(tr '\n' '|' <f2)"
run num 'set tags=tagsfile
tag tl
s/$/!/
w
q'
[ "$r" -eq 0 ] || pc_fail "tag with a line number: exit status $r"
printf 'alpha\nbeta\ngamma!\n' >exp
cmp -s f2 exp || pc_fail "the line number did not select the line: $(tr '\n' '|' <f2)"
run none 'set tags=tagsfile
tag nosuch
q'
[ "$r" -ne 0 ] || pc_fail "an unknown tag was accepted"
run order 'set tags=tagsnone tagsfile
tag tg
s/$/!/
w
q'
[ "$r" -eq 0 ] || pc_fail "second tags file: exit status $r"
printf 'alpha\nbeta!\ngamma\n' >exp
cmp -s f2 exp || pc_fail "the second tags file was not searched: $(tr '\n' '|' <f2)"
run len 'set tags=tagsfile taglength=2
tag tgxyz
s/$/!/
w
q'
[ "$r" -eq 0 ] || pc_fail "taglength: exit status $r"
cmp -s f2 exp || pc_fail "taglength did not limit the significant characters: $(tr '\n' '|' <f2)"
run big 'set tags=tagsfile
tag tbig
q'
[ "$r" -ne 0 ] || pc_fail "a tag with a line number past the end was accepted"
run nopat 'set tags=tagsfile
tag tnopat
q'
[ "$r" -ne 0 ] || pc_fail "a tag whose pattern does not match was accepted"
run full 'set tags=tagsfile
tag tgxyz
q'
[ "$r" -ne 0 ] || pc_fail "with taglength zero a longer name matched a shorter tag"
run mod 'set tags=tagsfile
1a
X
.
tag tg
q'
[ "$r" -ne 0 ] || pc_fail "tag from a modified buffer was accepted without !"
pc_pass

# shell program and expansions
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ex.043
. "$PC_CORE/pc.sh"
pc_begin XCU.ex.043
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

shp=
case ${PC_SH:-sh} in
/*) shp=$PC_SH ;;
*)
	oldifs=$IFS
	IFS=:
	for d in $PATH; do
		[ -n "$d" ] || d=.
		if [ -x "$d/${PC_SH:-sh}" ]; then
			shp=$d/${PC_SH:-sh}
			break
		fi
	done
	IFS=$oldifs
	case $shp in /*) ;; *) shp=$(cd "$(dirname "$shp")" && pwd)/$(basename "$shp") ;; esac
	;;
esac
[ -x "$shp" ] || pc_unresolved "cannot find the shell by an absolute path"

{
	printf '#!%s\n' "$shp"
	printf 'printf "%%s|%%s\\n" "$1" "$2" >>%s/wsh.log\n' "$PWD"
	printf 'exec %s "$@"\n' "$shp"
} >wsh
chmod 755 wsh || pc_unresolved "chmod"
five() { printf 'one\ntwo\nthree\nfour\nfive\n'; }
w=$PWD/wsh
pc_test
run() { # run NAME script ; log goes to wsh.log
	five >f
	: >wsh.log
	printf '%s\n' "$2" >s.$1
	ex -s f <s.$1 >o.$1 2>e.$1
	r=$?
}
run opt "set shell=$w
!echo hi >/dev/null
q!"
[ "$r" -eq 0 ] || pc_fail "shell option: exit status $r"
[ "$(cat wsh.log)" = '-c|echo hi >/dev/null' ] || pc_fail "the shell option program got '$(cat wsh.log)'"
SHELL=$w
export SHELL
run env '!echo hi >/dev/null
q!'
[ "$r" -eq 0 ] || pc_fail "SHELL: exit status $r"
[ "$(cat wsh.log)" = '-c|echo hi >/dev/null' ] || pc_fail "the program named by SHELL got '$(cat wsh.log)'"
run rd 'r !echo hi
q!'
[ "$(cat wsh.log)" = '-c|echo hi' ] || pc_fail "r ! passed '$(cat wsh.log)'"
run wr '1,2w !cat >/dev/null
q!'
[ "$(cat wsh.log)" = '-c|cat >/dev/null' ] || pc_fail "w ! passed '$(cat wsh.log)'"
run flt '1,2!cat
q!'
[ "$(cat wsh.log)" = '-c|cat' ] || pc_fail "filter passed '$(cat wsh.log)'"
run pc '!echo % >/dev/null
q!'
l=$(cat wsh.log)
case $l in
'-c|echo '?*' >/dev/null') case $l in *%*) pc_fail "% was not replaced in '$l'" ;; esac ;;
*) pc_fail "! with % passed '$l'" ;;
esac
run bang '!echo one >/dev/null
!!
q!'
[ "$(sed -n 1p wsh.log)" = '-c|echo one >/dev/null' ] || pc_fail "first command was '$(sed -n 1p wsh.log)'"
[ "$(sed -n 2p wsh.log)" = '-c|echo one >/dev/null' ] || pc_fail "!! passed '$(sed -n 2p wsh.log)'"
run esc '!echo a\!b >/dev/null
q!'
[ "$(cat wsh.log)" = '-c|echo a!b >/dev/null' ] || pc_fail "an escaped ! passed '$(cat wsh.log)'"
SHELL=
export SHELL
five >f
printf '!echo viash >out.sh\nq!\n' >s.sh
ex -s f <s.sh >o.sh 2>e.sh
[ "$(cat out.sh 2>/dev/null)" = viash ] || pc_fail "with SHELL empty the command did not run"
mkdir pbin || pc_unresolved "mkdir"
printf '#!%s\necho viapath\n' "$shp" >pbin/pcprog
chmod 755 pbin/pcprog || pc_unresolved "chmod"
SHELL=$shp
export SHELL
OPATH=$PATH
PATH=$PWD/pbin:$PATH
export PATH
printf '!pcprog >out.p\nq!\n' >s.p
ex -s f <s.p >o.p 2>e.p
PATH=$OPATH
export PATH
[ "$(cat out.p 2>/dev/null)" = viapath ] || pc_fail "the program was not found through PATH"
five >f
printf 'viash\n' >exp
printf '!echo a | tr a b >out.pipe\nq!\n' >s.pipe
ex -s f <s.pipe >o.pipe 2>e.pipe || pc_fail "! with a pipeline: exit status $?"
[ "$(cat out.pipe 2>/dev/null)" = b ] || pc_fail "the whole line after ! was not given to the shell"
printf '!!\nq!\n' >s.nb
ex -s f <s.nb >o.nb 2>e.nb
[ "$?" -ne 0 ] || pc_fail "!! with no earlier command was accepted"
printf '!echo hi\nq!\n' >s.hi
ex -s f <s.hi >o.hi 2>e.hi || pc_fail "! echo: exit status $?"
grep hi o.hi >/dev/null || pc_fail "the output of the program did not reach standard output"
printf '1,2w !cat >/dev/null; echo out; printf x\nq!\n' >s.wp
ex -s f <s.wp >o.wp 2>e.wp || pc_fail "w ! : exit status $?"
printf 'out\nx\n' >exp
cmp -s o.wp exp || pc_fail "the output of the program after w ! is '$(tr '\n' '|' <o.wp)'"
SHELL=$w
export SHELL
: >wsh.log
printf 'set shell=%s\nr y*\nq!\n' "$w" >s.glob
ex -s f <s.glob >o.glob 2>e.glob
[ "$(cat wsh.log)" = '-c|echo y*' ] || pc_fail "a file argument was expanded by '$(cat wsh.log)'"
{
	printf '#!%s\n' "$shp"
	printf 'printf "%%s\\n" "$1" >>%s/wsh2.log\n' "$PWD"
	printf 'exit 0\n'
} >wsh2
chmod 755 wsh2 || pc_unresolved "chmod"
five >f
printf 'set shell=%s\nsh\n1d\nw\nq\n' "$PWD/wsh2" >s.sh2
ex -s f <s.sh2 >o.sh2 2>e.sh2 || pc_fail "sh: exit status $?"
[ "$(cat wsh2.log 2>/dev/null)" = -i ] || pc_fail "the shell command passed '$(cat wsh2.log 2>/dev/null)', wanted -i"
printf 'two\nthree\nfour\nfive\n' >exp
cmp -s f exp || pc_fail "editing did not go on after the shell returned: $(tr '\n' '|' <f)"
pc_pass

# the prompt option
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ex.054
. "$PC_CORE/pc.sh"
pc_begin XCU.ex.054
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
. "$PC_CORE/pcpty.sh"
pc_pty_need
pc_pty_term
exrun() { # exrun INPUT ex-args...
	in=$1; shift
	"$PC_PTY" -t 25 -d 300 -i "$in" -o pty.out -- env "TERM=$PCT" "${PC_SH:-sh}" -c 'stty rows 24 cols 80; exec ex "$@"' ex "$@"
	rc=$?
	[ "$rc" -ne 125 ] || pc_unresolved "cannot set up a pseudo-terminal"
	[ "$rc" -ne 124 ] || pc_fail "ex did not finish"
}

printf 'a\nb\nc\n' >f
printf '2d\nw\nq\n' >in1
printf 'set noprompt\n2d\nw\nq\n' >in2
pc_test
exrun in1 f
n1=$(tr -cd ':' <pty.out | wc -c | tr -d ' ')
[ "$n1" -ge 3 ] || pc_fail "the default prompt was written $n1 times for three commands"
printf 'a\nb\nc\n' >f
exrun in2 f
n2=$(tr -cd ':' <pty.out | wc -c | tr -d ' ')
[ "$n2" -le 1 ] || pc_fail "with noprompt the prompt was written $n2 times"
printf 'a\nc\n' >exp
cmp -s f exp || pc_fail "the file is '$(tr '\n' '|' <f)'"
pc_pass

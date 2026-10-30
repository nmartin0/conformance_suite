# ex on a terminal
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ex.064
. "$PC_CORE/pc.sh"
pc_begin XCU.ex.064
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
	"$PC_PTY" -r -t 25 -d 300 -i "$in" -o pty.out -- env "TERM=$PCT" "${PC_SH:-sh}" -c 'stty rows 24 cols 80; exec ex "$@"' ex "$@"
	rc=$?
	[ "$rc" -ne 125 ] || pc_unresolved "cannot set up a pseudo-terminal"
	[ "$rc" -ne 124 ] || pc_fail "ex did not finish"
}

printf 'a\nb\nc\nd\ne\n' >f
printf '99d|1d\nw\nq\n' >in1
printf '!stty -a\nq\n' >in2
printf 'f\nq\n' >in3
printf 'q\n' >in4
cp f pcfile.txt
pc_test
exrun in1 f
printf 'a\nb\nc\nd\ne\n' >exp
cmp -s f exp || pc_fail "after an error the rest of the line was run: $(tr '\n' '|' <f)"
exrun in2 f
grep -E '(^|[[:space:]])icanon([[:space:]]|$)' pty.out >/dev/null 2>&1 || pc_fail "the terminal was not in canonical mode while ex read commands"
grep -E '(^|[[:space:]])-icanon([[:space:]]|$)' pty.out >/dev/null 2>&1 && pc_fail "canonical mode was off"
exrun in4 pcfile.txt
n0=$(grep -o 'pcfile.txt' pty.out | wc -l | tr -d ' ')
exrun in3 pcfile.txt
n1=$(grep -o 'pcfile.txt' pty.out | wc -l | tr -d ' ')
[ "$n1" -gt "$n0" ] || pc_fail "the file command did not write the pathname ($n1 against $n0 occurrences)"
pc_pass

# the autoindent option
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ex.056
. "$PC_CORE/pc.sh"
pc_begin XCU.ex.056
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
printf 'set ai\n2a\n\tX\nY\n.\nw\nq\n' >in1
printf 'set ai\n2a!\n\tX\nY\n.\nw\nq\n' >in2
pc_test
exrun in1 f
printf 'a\nb\n\tX\n\tY\nc\nd\ne\n' >exp
cmp -s f exp || pc_fail "with autoindent the file is '$(tr '\n\t' '|T' <f)'"
printf 'a\nb\nc\nd\ne\n' >f
exrun in2 f
printf 'a\nb\n\tX\nY\nc\nd\ne\n' >exp
cmp -s f exp || pc_fail "with a! the file is '$(tr '\n\t' '|T' <f)'"
printf 'a\nb\n\tc\nd\ne\n' >f
printf 'set ai\n3i\nX\n.\nw\nq\n' >in3
exrun in3 f
printf 'a\nb\n\tX\n\tc\nd\ne\n' >exp
cmp -s f exp || pc_fail "with i the file is '$(tr '\n\t' '|T' <f)'"
printf 'a\nb\n\tc\nd\ne\n' >f
printf 'set ai\n3c\nZ\n.\nw\nq\n' >in4
exrun in4 f
printf 'a\nb\n\tZ\nd\ne\n' >exp
cmp -s f exp || pc_fail "with c the file is '$(tr '\n\t' '|T' <f)'"
pc_pass

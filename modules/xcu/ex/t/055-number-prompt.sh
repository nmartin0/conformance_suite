# line numbers while entering text
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ex.055
. "$PC_CORE/pc.sh"
pc_begin XCU.ex.055
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

printf 'a\nb\nc\nd\ne\n' >f
printf 'set nu\n2a\nfoo\nbar\n.\nw\nq\n' >in
pc_test
exrun in f
printf 'a\nb\nfoo\nbar\nc\nd\ne\n' >exp
cmp -s f exp || pc_fail "the file is '$(tr '\n' '|' <f)'"
grep '^ *3  ' pty.out >/dev/null 2>&1 || grep '     3  ' pty.out >/dev/null 2>&1 || pc_fail "no prompt carried the line number 3"
grep '     4  ' pty.out >/dev/null 2>&1 || pc_fail "no prompt carried the line number 4"
pc_pass

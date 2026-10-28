# substitute with confirmation
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ex.059
. "$PC_CORE/pc.sh"
pc_begin XCU.ex.059
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

printf 'a b a\n' >f
printf 's/a/X/gc\ny\nn\nw\nq\n' >in
pc_test
exrun in f
printf 'X b a\n' >exp
cmp -s f exp || pc_fail "the file is '$(cat f)'"
grep '\^' pty.out >/dev/null 2>&1 || pc_fail "no caret line marked the text to be replaced"
pc_pass

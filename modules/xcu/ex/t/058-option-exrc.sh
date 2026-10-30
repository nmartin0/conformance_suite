# the exrc option
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ex.058
. "$PC_CORE/pc.sh"
pc_begin XCU.ex.058
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

mkdir h || pc_unresolved "mkdir"
printf 'a\nb\n' >f.orig
printf '1>\nw\nq\n' >in
printf 'set sw=4\n' >.exrc
chmod 644 .exrc
pc_test
HOME=$PWD/h
export HOME
unset EXINIT
cp f.orig f
exrun in f
l=$(sed -n 1p f)
[ "$l" != "    a" ] || pc_fail "the .exrc of the current directory was used without the exrc option"
[ "$l" != a ] || pc_fail "the > command did not shift the line"
EXINIT='set exrc'
export EXINIT
cp f.orig f
exrun in f
l=$(sed -n 1p f)
[ "$l" = "    a" ] || pc_fail "with the exrc option the first line is '$l', wanted four blanks and a"
pc_pass

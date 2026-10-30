# EXINIT and .exrc
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ex.057
. "$PC_CORE/pc.sh"
pc_begin XCU.ex.057
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
pc_test
try() { # try NAME expected-first-line
	cp f.orig f
	exrun in f
	l=$(sed -n 1p f)
	[ "$l" = "$2" ] || pc_fail "$1: the first line is '$l', wanted '$2'"
}
HOME=$PWD/h
export HOME
unset EXINIT
EXINIT='set sw=3'
export EXINIT
try "EXINIT" "   a"
unset EXINIT
printf '\nset sw=2\n\n' >h/.exrc
chmod 644 h/.exrc
try "HOME .exrc with blank lines" "  a"
EXINIT='set sw=3'
export EXINIT
try "EXINIT before .exrc" "   a"
unset EXINIT
chmod 666 h/.exrc
cp f.orig f
exrun in f
l=$(sed -n 1p f)
[ "$l" != "  a" ] || pc_fail "an .exrc writable by others was executed"
chmod 644 h/.exrc
HOME=
export HOME
cp f.orig f
exrun in f
l=$(sed -n 1p f)
[ "$l" != "  a" ] || pc_fail "the .exrc of an empty HOME was used"
pc_pass

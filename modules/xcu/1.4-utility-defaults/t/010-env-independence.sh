# The output and exit status of filters such as cat, sort and wc do not depend on unrelated environment variables defined by the standard, such as ENV, FCEDIT, HISTFILE, HISTSIZE, MAIL, MAILCHECK, MAILPATH, PS1, PS2, PS4, COLUMNS, LINES, CDPATH, OPTIND and OPTARG
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.1.4-utility-defaults.010
. "$PC_CORE/pc.sh"
pc_begin XCU.1.4-utility-defaults.010
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
printf 'b 2\na 1\nb 2\nc\tx\n' >in || pc_unresolved "cannot write the input"
pc_test
run() { env -i PATH="$PATH" LC_ALL=C "$@"; }
dirty() {
	env -i PATH="$PATH" LC_ALL=C ENV=/nonexistent/env FCEDIT=false HISTFILE=/nonexistent/h \
	    HISTSIZE=1 MAILCHECK=1 MAIL=/nonexistent/m MAILPATH=/nonexistent/m PS1='x ' PS2='y ' \
	    PS4='z ' COLUMNS=3 LINES=2 CDPATH=/ OPTIND=7 OPTARG=q OPTERR=0 LINENO=99 "$@"
}
cmpenv() {
	run "$@" <in >o1 2>e1; r1=$?
	dirty "$@" <in >o2 2>e2; r2=$?
	[ "$r1" = "$r2" ] || pc_fail "$*: exit status $r1 without the variables, $r2 with them"
	cmp -s o1 o2 || pc_fail "$*: the output depends on the environment"
	cmp -s e1 e2 || pc_fail "$*: the diagnostics depend on the environment"
}
cmpenv cat
cmpenv sort
cmpenv sort -u
cmpenv uniq
cmpenv wc
cmpenv wc -l
cmpenv head -n 1
cmpenv tail -n 1
cmpenv cut -c 1-2
cmpenv tr a-z A-Z
cmpenv fold -w 2
cmpenv paste - -
cmpenv od -c
cmpenv sed -n 2p
cmpenv awk '{print $1}'
cmpenv grep a
cmpenv expand
cmpenv expr 3 + 4
cmpenv basename /a/b
cmpenv printf '%s\n' x
cmpenv echo hi
pc_pass

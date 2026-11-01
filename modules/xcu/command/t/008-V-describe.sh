# command -V describes how a name is read.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.command.008
# MUTANT    command.V_util_no_path
# MUTANT    command.V_util_relative
# MUTANT    command.V_util_runs
# MUTANT    command.V_fn_no_word
# MUTANT    command.V_alias_no_word
# MUTANT    command.V_alias_no_value
# MUTANT    command.V_status1
# MUTANT    command.V_no_newline
# VARIANT   command.V_wording_a
# VARIANT   command.V_wording_b
#
# The format is unspecified: only the facts the page says the string
# carries are required, and a final newline (the page gives "%s\n"). The
# alias value is a single plain word so that any quoting shows it
# unbroken. The utility's pathname is looked for as a word of the line,
# with brackets and quotes taken off, and must name the utility.
. "$PC_CORE/pc.sh"
pc_begin XCU.command.008
mkdir bin || pc_fail "cannot make a directory"
printf ': >ran\n' >bin/pccmd || pc_fail "cannot make a script"
chmod +x bin/pccmd || pc_fail "cannot make the script executable"
base=$PWD
PATH=$base/bin:$PATH
pcfn() { :; }
alias pcal8=pcvalue8
pc_one() {
	# pc_one name: run command -V, leave the line in l1 and check the shape
	command -V "$1" >out 2>/dev/null
	rc=$?
	[ "$rc" -eq 0 ] || pc_fail "command -V $1 exited $rc"
	pc_need_file out "the output of command -V"
	{ IFS= read -r l1; r1=$?; } <out
	[ "$r1" -eq 0 ] || pc_fail "command -V $1 wrote '$l1' without a final newline (or nothing)"
}
pc_test
pc_one pccmd
found=
for w in $l1; do
	for pcq in '(' '"' "'" '`'; do w=${w#"$pcq"}; done
	for pcq in ')' '"' "'" '`' ',' ':' ';'; do w=${w%"$pcq"}; done
	case $w in
	/*) if [ "$w" -ef "$base/bin/pccmd" ]; then found=1; fi ;;
	esac
done
[ -n "$found" ] || pc_fail "command -V for a utility wrote '$l1', without its absolute pathname"
[ ! -e ran ] || pc_fail "command -V ran the utility"
pc_one pcfn
case $l1 in
*[Ff]unction*) ;;
*) pc_fail "command -V for a function wrote '$l1', which does not say function" ;;
esac
pc_one pcal8
case $l1 in
*[Aa]lias*) ;;
*) pc_fail "command -V for an alias wrote '$l1', which does not say alias" ;;
esac
case $l1 in
*pcvalue8*) ;;
*) pc_fail "command -V for an alias wrote '$l1', without the value" ;;
esac
pc_pass

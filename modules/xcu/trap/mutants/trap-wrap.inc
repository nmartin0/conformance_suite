# Shared by the trap faults and variants (not itself a fault).
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# trap is a special built-in of the shell under test, and the tests run
# their scripts in a shell of their own (the program PC_SH names). A fault
# or variant therefore cannot be a function in the test's own shell. Instead
# this file makes PC_SH a small wrapper program: it starts the real shell,
# which first reads pc-mut.def (the helpers below followed by the fault's
# own definitions) and then runs the script. The fault's definitions replace
# the built-in by an alias for a function, whose "command trap" reaches the
# real built-in.
#
# The fault file writes its definitions to pc-mut.part and then sources this
# file. A definition may also set pcm_sed, a sed script applied to every
# script before it runs (used to swap one-line helper functions that the
# tests define at the start of a line, such as pcfg and pcsub).
pcm_real=$(command -v "${PC_SH:-sh}") || pc_unresolved "cannot find the shell under test"
pcm_dir=$PWD
{
	printf 'pcm_real=%s\n' "$pcm_real"
	printf 'pcm_dir=%s\n' "$pcm_dir"
	cat <<'PCM_HELP'
pcm_nl='
'
# pcm_split ARGS: classify a call of trap. pcm_kind is one of
#   list  no operands          p    first argument -p
#   set   action to run        null empty action (ignore)
#   hyphen  action - (reset)     num  first operand all digits (reset)
# pcm_n is how many leading arguments come before the conditions (for kinds
# set, null, hyphen), pcm_act the action, pcm_dd set when -- was given.
pcm_split() {
	pcm_n=0 pcm_kind=list pcm_act= pcm_dd=
	if [ $# -gt 0 ] && [ "$1" = -p ]; then pcm_kind=p; pcm_n=1; return 0; fi
	if [ $# -gt 0 ] && [ "$1" = -- ]; then pcm_dd=1; pcm_n=1; shift; fi
	[ $# -gt 0 ] || return 0
	pcm_act=$1
	case $1 in
	'') pcm_kind=null; pcm_n=$((pcm_n + 1)) ;;
	-) pcm_kind=hyphen; pcm_n=$((pcm_n + 1)) ;;
	*[!0-9]*) pcm_kind=set; pcm_n=$((pcm_n + 1)) ;;
	*) pcm_kind=num ;;
	esac
	return 0
}
# pcm_key NAME: NAME as a string fit for a variable name.
pcm_key() { printf '%s' "$1" | tr -c 'A-Za-z0-9' _; }
# pcm_get / pcm_put: a small table of strings keyed by condition.
pcm_get() { eval "pcm_val=\${pcm_t_$(pcm_key "$1")-}"; }
pcm_put() { eval "pcm_t_$(pcm_key "$1")=\$2"; }
# pcm_conform ARGS: trap -p that also writes a line for a condition in its
# default state (trap -- - COND), the form the 2024 text asks for.
pcm_conform() {
	shift
	if [ $# -eq 0 ]; then set -- HUP INT QUIT ABRT ALRM TERM USR1 USR2 PIPE CHLD EXIT; fi
	for pcm_c; do
		command trap -p "$pcm_c" >"$pcm_dir/pc-mut.pl" 2>/dev/null
		if [ -s "$pcm_dir/pc-mut.pl" ]; then
			cat "$pcm_dir/pc-mut.pl"
		else
			printf 'trap -- - %s\n' "$pcm_c"
		fi
	done
}
pcm_trap() { command trap "$@"; }
PCM_HELP
	cat pc-mut.part
	printf '%s\n' 'alias trap=pcm_trap'
} >pc-mut.def || pc_unresolved "cannot write pc-mut.def"
{
	printf '#!%s\n' "$pcm_real"
	printf '%s\n' 'command -v shopt >/dev/null 2>&1 && shopt -s expand_aliases'
	printf '. %s/pc-mut.def\n' "$pcm_dir"
	cat <<'PCM_WRAP'
pcm_s=$1
shift
case $pcm_s in */*) ;; *) pcm_s=./$pcm_s ;; esac
if [ -n "${pcm_sed-}" ]; then
	sed "$pcm_sed" "$pcm_s" >"$pcm_dir/pc-mut.rs.$$" && pcm_s=$pcm_dir/pc-mut.rs.$$
fi
. "$pcm_s"
PCM_WRAP
} >pc-mutsh || pc_unresolved "cannot write pc-mutsh"
chmod +x pc-mutsh
PC_SH=$pcm_dir/pc-mutsh
export PC_SH

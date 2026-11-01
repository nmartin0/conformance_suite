# testref.sh - shared engine of the planted faults for test and [ (sourced
# by the fault files of this directory; not a fault itself).
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# pcT_run FAULT ARG... evaluates the expression ARG... as the real
# evaluator does, except that one requirement is broken. A fault file plants
# the fault three ways, chosen by the letters given to pcT_plant:
#   f  a shell function named test (what a call by the shell reaches)
#   t  a standalone program test first on PATH (what a direct exec reaches)
#   b  a standalone program [ first on PATH (same, for the second name)
# The shell functions cannot cover [ : a function of that name is not
# possible in every shell, so a call of [ by the shell is never faulted.

if [ -n "${PC_MUTANT_FILE:-}" ]; then
	pcT_dir=$(cd "${PC_MUTANT_FILE%/*}" && pwd)
fi

pcT_t() { command test "$@"; }

# pcT_lc / pcT_sw: the text of a string in lower case / with case swapped
pcT_lc() { printf '%s' "$1" | tr 'A-Z' 'a-z'; }
pcT_sw() { printf '%s' "$1" | tr 'A-Za-z' 'a-zA-Z'; }

# pcT_lex OP A B: compare A and B as strings (OP is gt, ge, lt or le)
pcT_lex() {
	awk -v a="$2" -v b="$3" -v op="$1" 'BEGIN {
		x = a ""; y = b ""
		if (op == "gt") r = (x > y); else if (op == "ge") r = (x >= y)
		else if (op == "lt") r = (x < y); else r = (x <= y)
		exit !r }'
}

pcT_run() {
	pcT_f=$1
	shift
	pcT_n=$#
	pcT_a=${1-}
	pcT_b=${2-}
	pcT_c=${3-}
	case $pcT_f in
	# --- no argument, one argument
	zero_true) case $pcT_n in 0) return 0 ;; esac ;;
	zero_status2) case $pcT_n in 0) return 2 ;; esac ;;
	one_empty_true) case $pcT_n:$pcT_a in 1:) return 0 ;; esac ;;
	one_op_false) case $pcT_n:$pcT_a in 1:-n | 1:-z | 1:-f | 1:-e | 1:-d | 1:'!' | 1:=) return 1 ;; esac ;;
	one_dash_false) case $pcT_n:$pcT_a in 1:-*) return 1 ;; esac ;;
	one_bang_false) case $pcT_n:$pcT_a in 1:'!') return 1 ;; esac ;;
	one_blank_false) case $pcT_n:$pcT_a in 1:' ') return 1 ;; esac ;;
	# --- -n and -z
	n_z_swap)
		case $pcT_n:$pcT_a in
		2:-n) pcT_t -z "$pcT_b"; return ;;
		2:-z) pcT_t -n "$pcT_b"; return ;;
		esac ;;
	n_empty_true) case $pcT_n:$pcT_a:$pcT_b in 2:-n:) return 0 ;; esac ;;
	z_nonempty_true) case $pcT_n:$pcT_a:$pcT_b in 2:-z:?*) return 0 ;; esac ;;
	n_opstring_false) case $pcT_n:$pcT_a:$pcT_b in 2:-n:-n | 2:-n:-z | 2:-n:= | 2:-n:'!=' | 2:-n:'!' | 2:-n:-f | 2:-n:-e) return 1 ;; esac ;;
	z_opstring_true) case $pcT_n:$pcT_a:$pcT_b in 2:-z:-n | 2:-z:-z | 2:-z:= | 2:-z:'!=' | 2:-z:'!' | 2:-z:-f | 2:-z:-e) return 0 ;; esac ;;
	n_blank_false) case $pcT_n:$pcT_a:$pcT_b in 2:-n:' ') return 1 ;; esac ;;
	# --- = and !=
	eq_ne_swap)
		case $pcT_n:$pcT_b in
		3:=) pcT_t "$pcT_a" '!=' "$pcT_c"; return ;;
		3:'!=') pcT_t "$pcT_a" = "$pcT_c"; return ;;
		esac ;;
	eq_prefix)
		case $pcT_n:$pcT_b in
		3:=)
			case $pcT_a in ?*) case $pcT_c in ?*)
				case $pcT_a in "$pcT_c"*) return 0 ;; esac
				case $pcT_c in "$pcT_a"*) return 0 ;; esac
			;; esac ;; esac ;;
		esac ;;
	eq_empty_true)
		case $pcT_n:$pcT_b in
		3:=) case $pcT_a in '') return 0 ;; esac; case $pcT_c in '') return 0 ;; esac ;;
		esac ;;
	ne_empty_false)
		case $pcT_n:$pcT_b in
		3:'!=') case $pcT_a in '') return 1 ;; esac; case $pcT_c in '') return 1 ;; esac ;;
		esac ;;
	eq_nocase)
		case $pcT_n:$pcT_b in
		3:=) pcT_t "$(pcT_lc "$pcT_a")" = "$(pcT_lc "$pcT_c")"; return ;;
		esac ;;
	eq_first2)
		case $pcT_n:$pcT_b in
		3:=) pcT_t "$(printf '%.2s' "$pcT_a")" = "$(printf '%.2s' "$pcT_c")"; return ;;
		esac ;;
	# --- < and >
	lt_gt_swap)
		case $pcT_n:$pcT_b in
		3:'<') pcT_t "$pcT_a" '>' "$pcT_c"; return ;;
		3:'>') pcT_t "$pcT_a" '<' "$pcT_c"; return ;;
		esac ;;
	lt_nocase)
		case $pcT_n:$pcT_b in
		3:'<' | 3:'>') pcT_t "$(pcT_lc "$pcT_a")" "$pcT_b" "$(pcT_lc "$pcT_c")"; return ;;
		esac ;;
	lt_case_inverted)
		case $pcT_n:$pcT_b in
		3:'<' | 3:'>') pcT_t "$(pcT_sw "$pcT_a")" "$pcT_b" "$(pcT_sw "$pcT_c")"; return ;;
		esac ;;
	lt_len)
		case $pcT_n:$pcT_b in
		3:'<' | 3:'>')
			if [ "${#pcT_a}" -ne "${#pcT_c}" ]; then
				if [ "${#pcT_a}" -lt "${#pcT_c}" ]; then pcT_r='<'; else pcT_r='>'; fi
				if [ "$pcT_r" = "$pcT_b" ]; then return 0; else return 1; fi
			fi ;;
		esac ;;
	lt_le)
		case $pcT_n:$pcT_b in
		3:'<' | 3:'>') case $pcT_a in "$pcT_c") return 0 ;; esac ;;
		esac ;;
	lt_prefix_false)
		case $pcT_n:$pcT_b in
		3:'<' | 3:'>')
			case $pcT_a in "$pcT_c"*) return 1 ;; esac
			case $pcT_c in "$pcT_a"*) return 1 ;; esac ;;
		esac ;;
	lt_first_only)
		case $pcT_n:$pcT_b in
		3:'<' | 3:'>') pcT_t "$(printf '%.1s' "$pcT_a")" "$pcT_b" "$(printf '%.1s' "$pcT_c")"; return ;;
		esac ;;
	# --- integers
	int_lexical)
		case $pcT_n:$pcT_b in
		3:-gt) pcT_lex gt "$pcT_a" "$pcT_c"; return ;;
		3:-ge) pcT_lex ge "$pcT_a" "$pcT_c"; return ;;
		3:-lt) pcT_lex lt "$pcT_a" "$pcT_c"; return ;;
		3:-le) pcT_lex le "$pcT_a" "$pcT_c"; return ;;
		esac ;;
	int_abs)
		case $pcT_n:$pcT_b in
		3:-eq | 3:-ne | 3:-gt | 3:-ge | 3:-lt | 3:-le) pcT_t "${pcT_a#-}" "$pcT_b" "${pcT_c#-}"; return ;;
		esac ;;
	int_ge_as_gt) case $pcT_n:$pcT_b in 3:-ge) pcT_t "$pcT_a" -gt "$pcT_c"; return ;; esac ;;
	int_le_as_lt) case $pcT_n:$pcT_b in 3:-le) pcT_t "$pcT_a" -lt "$pcT_c"; return ;; esac ;;
	int_gt_as_ge) case $pcT_n:$pcT_b in 3:-gt) pcT_t "$pcT_a" -ge "$pcT_c"; return ;; esac ;;
	int_lt_as_le) case $pcT_n:$pcT_b in 3:-lt) pcT_t "$pcT_a" -le "$pcT_c"; return ;; esac ;;
	int_ne_eq_swap)
		case $pcT_n:$pcT_b in
		3:-eq) pcT_t "$pcT_a" -ne "$pcT_c"; return ;;
		3:-ne) pcT_t "$pcT_a" -eq "$pcT_c"; return ;;
		esac ;;
	int_gt_lt_swap)
		case $pcT_n:$pcT_b in
		3:-gt) pcT_t "$pcT_a" -lt "$pcT_c"; return ;;
		3:-lt) pcT_t "$pcT_a" -gt "$pcT_c"; return ;;
		3:-ge) pcT_t "$pcT_a" -le "$pcT_c"; return ;;
		3:-le) pcT_t "$pcT_a" -ge "$pcT_c"; return ;;
		esac ;;
	int_mod256)
		case $pcT_n:$pcT_b in
		3:-eq | 3:-ne | 3:-gt | 3:-ge | 3:-lt | 3:-le) pcT_t "$((pcT_a % 256))" "$pcT_b" "$((pcT_c % 256))"; return ;;
		esac ;;
	# --- file types
	e_dir_false) case $pcT_n:$pcT_a in 2:-e) pcT_t -d "$pcT_b" && return 1 ;; esac ;;
	e_fifo_false) case $pcT_n:$pcT_a in 2:-e) pcT_t -p "$pcT_b" && return 1 ;; esac ;;
	e_none_true) case $pcT_n:$pcT_a in 2:-e) pcT_t -e "$pcT_b" || return 0 ;; esac ;;
	f_dir_true) case $pcT_n:$pcT_a in 2:-f) pcT_t -d "$pcT_b" && return 0 ;; esac ;;
	f_fifo_true) case $pcT_n:$pcT_a in 2:-f) pcT_t -p "$pcT_b" && return 0 ;; esac ;;
	f_none_true) case $pcT_n:$pcT_a in 2:-f) pcT_t -e "$pcT_b" || return 0 ;; esac ;;
	f_reg_false) case $pcT_n:$pcT_a in 2:-f) pcT_t -f "$pcT_b" && return 1 ;; esac ;;
	d_nondir_true) case $pcT_n:$pcT_a in 2:-d) pcT_t -e "$pcT_b" && return 0 ;; esac ;;
	d_dir_false) case $pcT_n:$pcT_a in 2:-d) pcT_t -d "$pcT_b" && return 1 ;; esac ;;
	d_none_true) case $pcT_n:$pcT_a in 2:-d) pcT_t -e "$pcT_b" || return 0 ;; esac ;;
	L_dangling_false) case $pcT_n:$pcT_a in 2:-L) pcT_t -e "$pcT_b" || return 1 ;; esac ;;
	L_exists_true) case $pcT_n:$pcT_a in 2:-L) pcT_t -e "$pcT_b" && return 0 ;; esac ;;
	L_none_true) case $pcT_n:$pcT_a in 2:-L) pcT_t -e "$pcT_b" || pcT_t -L "$pcT_b" || return 0 ;; esac ;;
	h_dangling_false) case $pcT_n:$pcT_a in 2:-h) pcT_t -e "$pcT_b" || return 1 ;; esac ;;
	h_exists_true) case $pcT_n:$pcT_a in 2:-h) pcT_t -e "$pcT_b" && return 0 ;; esac ;;
	p_dir_true) case $pcT_n:$pcT_a in 2:-p) pcT_t -d "$pcT_b" && return 0 ;; esac ;;
	p_reg_true) case $pcT_n:$pcT_a in 2:-p) pcT_t -f "$pcT_b" && return 0 ;; esac ;;
	p_fifo_false) case $pcT_n:$pcT_a in 2:-p) pcT_t -p "$pcT_b" && return 1 ;; esac ;;
	u_exists_true) case $pcT_n:$pcT_a in 2:-u) pcT_t -e "$pcT_b" && return 0 ;; esac ;;
	u_as_g) case $pcT_n:$pcT_a in 2:-u) pcT_t -g "$pcT_b"; return ;; esac ;;
	u_false) case $pcT_n:$pcT_a in 2:-u) return 1 ;; esac ;;
	g_exists_true) case $pcT_n:$pcT_a in 2:-g) pcT_t -e "$pcT_b" && return 0 ;; esac ;;
	g_as_u) case $pcT_n:$pcT_a in 2:-g) pcT_t -u "$pcT_b"; return ;; esac ;;
	g_false) case $pcT_n:$pcT_a in 2:-g) return 1 ;; esac ;;
	# --- size and access
	s_empty_true) case $pcT_n:$pcT_a in 2:-s) pcT_t -e "$pcT_b" && return 0 ;; esac ;;
	s_full_false) case $pcT_n:$pcT_a in 2:-s) pcT_t -s "$pcT_b" && return 1 ;; esac ;;
	s_none_true) case $pcT_n:$pcT_a in 2:-s) pcT_t -e "$pcT_b" || return 0 ;; esac ;;
	r_missing_true) case $pcT_n:$pcT_a in 2:-r) pcT_t -e "$pcT_b" || return 0 ;; esac ;;
	w_missing_true) case $pcT_n:$pcT_a in 2:-w) pcT_t -e "$pcT_b" || return 0 ;; esac ;;
	x_missing_true) case $pcT_n:$pcT_a in 2:-x) pcT_t -e "$pcT_b" || return 0 ;; esac ;;
	r_denied_true) case $pcT_n:$pcT_a in 2:-r) pcT_t -e "$pcT_b" && return 0 ;; esac ;;
	w_denied_true) case $pcT_n:$pcT_a in 2:-w) pcT_t -e "$pcT_b" && return 0 ;; esac ;;
	x_denied_true) case $pcT_n:$pcT_a in 2:-x) pcT_t -e "$pcT_b" && return 0 ;; esac ;;
	r_granted_false) case $pcT_n:$pcT_a in 2:-r) pcT_t -r "$pcT_b" && return 1 ;; esac ;;
	w_granted_false) case $pcT_n:$pcT_a in 2:-w) pcT_t -w "$pcT_b" && return 1 ;; esac ;;
	x_granted_false) case $pcT_n:$pcT_a in 2:-x) pcT_t -x "$pcT_b" && return 1 ;; esac ;;
	x_dir_false) case $pcT_n:$pcT_a in 2:-x) pcT_t -d "$pcT_b" && return 1 ;; esac ;;
	r_dir_false) case $pcT_n:$pcT_a in 2:-r) pcT_t -d "$pcT_b" && return 1 ;; esac ;;
	w_dir_false) case $pcT_n:$pcT_a in 2:-w) pcT_t -d "$pcT_b" && return 1 ;; esac ;;
	r_as_w) case $pcT_n:$pcT_a in 2:-r) pcT_t -w "$pcT_b"; return ;; esac ;;
	w_as_x) case $pcT_n:$pcT_a in 2:-w) pcT_t -x "$pcT_b"; return ;; esac ;;
	x_as_r) case $pcT_n:$pcT_a in 2:-x) pcT_t -r "$pcT_b"; return ;; esac ;;
	# --- times and same file
	nt_ot_swap)
		case $pcT_n:$pcT_b in
		3:-nt) pcT_t "$pcT_a" -ot "$pcT_c"; return ;;
		3:-ot) pcT_t "$pcT_a" -nt "$pcT_c"; return ;;
		esac ;;
	nt_missing_false) case $pcT_n:$pcT_b in 3:-nt) pcT_t -e "$pcT_c" || return 1 ;; esac ;;
	ot_missing_false) case $pcT_n:$pcT_b in 3:-ot) pcT_t -e "$pcT_a" || return 1 ;; esac ;;
	nt_missing_first_true) case $pcT_n:$pcT_b in 3:-nt) pcT_t -e "$pcT_a" || return 0 ;; esac ;;
	ot_missing_second_true) case $pcT_n:$pcT_b in 3:-ot) pcT_t -e "$pcT_c" || return 0 ;; esac ;;
	nt_equal_true) case $pcT_n:$pcT_b in 3:-nt) pcT_t ! "$pcT_a" -ot "$pcT_c"; return ;; esac ;;
	ot_equal_true) case $pcT_n:$pcT_b in 3:-ot) pcT_t ! "$pcT_a" -nt "$pcT_c"; return ;; esac ;;
	nt_ctime | nt_atime | ot_ctime | ot_atime)
		case $pcT_n:$pcT_b in
		3:-nt | 3:-ot)
			if pcT_t -e "$pcT_a" && pcT_t -e "$pcT_c"; then
				case $pcT_f in *_ctime) pcT_o=-tc ;; *) pcT_o=-tu ;; esac
				case $pcT_a in "$pcT_c") return 1 ;; esac
				pcT_first=$(ls -d $pcT_o -- "$pcT_a" "$pcT_c" | { read -r pcT_x; printf '%s' "$pcT_x"; })
				case $pcT_b in
				-nt) pcT_w=$pcT_a ;;
				*) pcT_w=$pcT_c ;;
				esac
				case $pcT_first in "$pcT_w") return 0 ;; *) return 1 ;; esac
			fi ;;
		esac ;;
	ef_names)
		case $pcT_n:$pcT_b in
		3:-ef) case $pcT_a in "$pcT_c") pcT_t -e "$pcT_a" && return 0 ;; esac; return 1 ;;
		esac ;;
	ef_nofollow)
		case $pcT_n:$pcT_b in
		3:-ef) pcT_t -h "$pcT_a" && return 1; pcT_t -h "$pcT_c" && return 1 ;;
		esac ;;
	ef_size)
		case $pcT_n:$pcT_b in
		3:-ef)
			if pcT_t -f "$pcT_a" && pcT_t -f "$pcT_c"; then
				pcT_sa=$(wc -c <"$pcT_a"); pcT_sc=$(wc -c <"$pcT_c")
				case $pcT_sa in "$pcT_sc") return 0 ;; *) return 1 ;; esac
			fi ;;
		esac ;;
	ef_missing_true) case $pcT_n:$pcT_b in 3:-ef) pcT_t -e "$pcT_a" && pcT_t -e "$pcT_c" || return 0 ;; esac ;;
	ef_hardlink_false)
		case $pcT_n:$pcT_b in
		3:-ef) case $pcT_a in "$pcT_c") ;; *) pcT_t -h "$pcT_a" || pcT_t -h "$pcT_c" || return 1 ;; esac ;;
		esac ;;
	# --- links are followed
	f_nofollow) case $pcT_n:$pcT_a in 2:-f) pcT_t -h "$pcT_b" && return 1 ;; esac ;;
	s_nofollow) case $pcT_n:$pcT_a in 2:-s) pcT_t -h "$pcT_b" && return 1 ;; esac ;;
	d_nofollow) case $pcT_n:$pcT_a in 2:-d) pcT_t -h "$pcT_b" && return 1 ;; esac ;;
	e_nofollow) case $pcT_n:$pcT_a in 2:-e) pcT_t -h "$pcT_b" && return 0 ;; esac ;;
	f_link_true) case $pcT_n:$pcT_a in 2:-f) pcT_t -h "$pcT_b" && return 0 ;; esac ;;
	d_link_true) case $pcT_n:$pcT_a in 2:-d) pcT_t -h "$pcT_b" && return 0 ;; esac ;;
	# --- negation
	neg_ignored) case $pcT_n:$pcT_a in [234]:'!') shift; pcT_t "$@"; return ;; esac ;;
	neg_two_ignored) case $pcT_n:$pcT_a in 2:'!') shift; pcT_t "$@"; return ;; esac ;;
	neg_three_ignored) case $pcT_n:$pcT_a in 3:'!') shift; pcT_t "$@"; return ;; esac ;;
	neg_four_ignored) case $pcT_n:$pcT_a in 4:'!') shift; pcT_t "$@"; return ;; esac ;;
	neg_empty_false) case $pcT_n:$pcT_a:$pcT_b in 2:'!':) return 1 ;; esac ;;
	neg_nonempty_true) case $pcT_n:$pcT_a:$pcT_b in 2:'!':?*) return 0 ;; esac ;;
	neg_wins_bin) case $pcT_n:$pcT_a:$pcT_b in 3:'!':= | 3:'!':'!=') return 1 ;; esac ;;
	neg_loses_bin) case $pcT_n:$pcT_a:$pcT_b in 3:'!':= | 3:'!':'!=') return 0 ;; esac ;;
	# --- -t
	t_open_true) case $pcT_n:$pcT_a in 2:-t) ( eval ": <&$pcT_b" ) 2>/dev/null && return 0 ;; esac ;;
	t_unopen_true) case $pcT_n:$pcT_a in 2:-t) ( eval ": <&$pcT_b" ) 2>/dev/null || return 0 ;; esac ;;
	t_always_true) case $pcT_n:$pcT_a in 2:-t) return 0 ;; esac ;;
	# --- the argument --
	dd_leading_dropped) case $pcT_a in --) shift; pcT_t "$@"; return ;; esac ;;
	dd_all_dropped)
		pcT_i=0
		while [ "$pcT_i" -lt "$pcT_n" ]; do
			pcT_x=$1
			shift
			case $pcT_x in --) ;; *) set -- "$@" "$pcT_x" ;; esac
			pcT_i=$((pcT_i + 1))
		done
		pcT_t "$@"
		return ;;
	# --- standard output
	stdout_true) pcT_t "$@"; pcT_r=$?; case $pcT_r in 0) printf 'true\n' ;; esac; return $pcT_r ;;
	stdout_false) pcT_t "$@"; pcT_r=$?; case $pcT_r in 1) printf 'false\n' ;; esac; return $pcT_r ;;
	stdout_newline) pcT_t "$@"; pcT_r=$?; printf '\n'; return $pcT_r ;;
	# --- locale (cannot be judged where no locale can be made)
	coll_bytes)
		case $pcT_n:$pcT_b in
		3:'<') LC_ALL=POSIX expr "$pcT_a" '<' "$pcT_c" >/dev/null; case $? in 0) return 0 ;; *) return 1 ;; esac ;;
		3:'>') LC_ALL=POSIX expr "$pcT_a" '>' "$pcT_c" >/dev/null; case $? in 0) return 0 ;; *) return 1 ;; esac ;;
		esac ;;
	*) printf 'pcT_run: unknown fault %s\n' "$pcT_f" >&2; return 99 ;;
	esac
	pcT_t "$@"
}

# pcT_br FAULT ARG... ] : as pcT_run for the form [ ARG... ]
pcT_br() {
	pcT_g=$1
	shift
	pcT_k=$#
	pcT_i=1
	while [ "$pcT_i" -lt "$pcT_k" ]; do
		set -- "$@" "$1"
		shift
		pcT_i=$((pcT_i + 1))
	done
	if [ "$pcT_k" -lt 1 ] || [ "$1" != ] ]; then
		printf '[: missing ]\n' >&2
		return 2
	fi
	shift
	pcT_run "$pcT_g" "$@"
}

# pcT_plant FAULT WHERE : plant the fault (WHERE: letters f, t, b)
pcT_plant() {
	pcT_cur=$1
	case $2 in *f*) test() { pcT_run "$pcT_cur" "$@"; } ;; esac
	case $2 in *[tb]*) mkdir -p pc-mutbin ;; esac
	case $2 in
	*t*)
		printf '#!/bin/sh\n. "%s/testref.sh"\npcT_run %s "$@"\nexit $?\n' "$pcT_dir" "$1" >pc-mutbin/test
		chmod +x pc-mutbin/test ;;
	esac
	case $2 in
	*b*)
		printf '#!/bin/sh\n. "%s/testref.sh"\npcT_br %s "$@"\nexit $?\n' "$pcT_dir" "$1" >'pc-mutbin/['
		chmod +x 'pc-mutbin/[' ;;
	esac
	case $2 in *[tb]*) PATH=$PWD/pc-mutbin:$PATH ;; esac
}

# aliasref.sh - shared body of the alias faults and variants.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# A fault or variant file sets PCM_FAULT and sources this file. Shell
# functions shadow alias (and eval, for faults about substitution); the real
# utilities are reached with "command". With a name that no case below
# knows, everything behaves as the real utility does.
pcm_nl='
'
pcm_tab=$(command printf '\t')
pcm_realsh=${PC_SH:-sh}
pcm_pf=pcm_pid.$$
pcm_store=pcm_store.$$
pcm_redefs=

pcm_okname() { case $1 in '' | *[!A-Za-z0-9_]*) return 1 ;; esac; return 0; }

# pcm_sq / pcm_dq / pcm_bq VALUE: set pcm_q to the value quoted three ways.
pcm_sq() {
	pcm_s=$1 pcm_o=
	while :; do
		case $pcm_s in
		*"'"*) pcm_o=$pcm_o${pcm_s%%"'"*}"'\\''"; pcm_s=${pcm_s#*"'"} ;;
		*) break ;;
		esac
	done
	pcm_q="'$pcm_o$pcm_s'"
}
pcm_dq() {
	pcm_s=$1 pcm_o=
	while [ -n "$pcm_s" ]; do
		pcm_c=${pcm_s%"${pcm_s#?}"}
		pcm_s=${pcm_s#?}
		case $pcm_c in
		'\' | '"' | '$' | '`') pcm_o="$pcm_o\\$pcm_c" ;;
		*) pcm_o=$pcm_o$pcm_c ;;
		esac
	done
	pcm_q="\"$pcm_o\""
}
pcm_bq() {
	pcm_s=$1 pcm_o=
	while [ -n "$pcm_s" ]; do
		pcm_c=${pcm_s%"${pcm_s#?}"}
		pcm_s=${pcm_s#?}
		case $pcm_c in
		[A-Za-z0-9_]) pcm_o=$pcm_o$pcm_c ;;
		"$pcm_nl") pcm_o="$pcm_o'$pcm_nl'" ;;
		*) pcm_o="$pcm_o\\$pcm_c" ;;
		esac
	done
	pcm_q=$pcm_o
}
# pcm_del CHAR TEXT: pcm_dres is TEXT without CHAR.
pcm_del() {
	pcm_dres= pcm_ds=$2
	while :; do
		case $pcm_ds in
		*"$1"*) pcm_dres=$pcm_dres${pcm_ds%%"$1"*}; pcm_ds=${pcm_ds#*"$1"} ;;
		*) break ;;
		esac
	done
	pcm_dres=$pcm_dres$pcm_ds
}
# pcm_repl CHAR WITH TEXT: pcm_dres is TEXT with CHAR replaced.
pcm_repl() {
	pcm_dres= pcm_ds=$3
	while :; do
		case $pcm_ds in
		*"$1"*) pcm_dres=$pcm_dres${pcm_ds%%"$1"*}$2; pcm_ds=${pcm_ds#*"$1"} ;;
		*) break ;;
		esac
	done
	pcm_dres=$pcm_dres$pcm_ds
}

# pcm_get NAME: pcm_gv is the value of the real alias; fails if missing.
pcm_get() {
	pcm_g=$(command alias "$1" 2>/dev/null && command printf x) || return 1
	pcm_g=${pcm_g%x}
	pcm_g=${pcm_g%"$pcm_nl"}
	pcm_rest=${pcm_g#*=}
	eval "set -- $pcm_rest"
	pcm_gv=$1
	return 0
}

pcm_emit() { # name value style
	case $3 in
	dq) pcm_dq "$2" ;;
	bq) pcm_bq "$2" ;;
	*) pcm_sq "$2" ;;
	esac
	command printf '%s=%s\n' "$1" "$pcm_q"
}

pcm_pid() {
	command "$pcm_realsh" -c 'echo $PPID' >"$pcm_pf" 2>/dev/null
	read -r pcm_cur <"$pcm_pf"
}
pcm_insub() { pcm_pid; [ "$pcm_cur" != "$pcm_top" ]; }
pcm_pid
pcm_top=$pcm_cur

pcm_sync() {
	if [ "${PCM_FAULT-}" = sub_leaks ] && [ -f "$pcm_store" ]; then
		while IFS= read -r pcm_sl; do
			command alias "$pcm_sl"
		done <"$pcm_store"
	fi
	return 0
}

# A shell run from this one, with this shell's aliases handed over.
pcm_shw() {
	pcm_pre=
	pcm_all=$(command alias)
	while IFS= read -r pcm_l; do
		if [ -n "$pcm_l" ]; then pcm_pre="${pcm_pre}alias $pcm_l$pcm_nl"; fi
	done <<PCM_EOF
$pcm_all
PCM_EOF
	if [ "$1" = -c ]; then
		pcm_cmd=$2
		shift 2
		command "$pcm_realsh" -c "$pcm_pre$pcm_cmd" "$@"
	else
		pcm_f=$1
		shift
		{ command printf '%s' "$pcm_pre"; command cat "$pcm_f"; } >pcm_shw.$$
		command "$pcm_realsh" pcm_shw.$$ "$@"
		pcm_r=$?
		command rm -f pcm_shw.$$
		return $pcm_r
	fi
}

pcm_def() {
	pcm_n=${1%%=*} pcm_v=${1#*=}
	pcm_have=0
	if command alias "$pcm_n" >/dev/null 2>&1; then pcm_have=1; fi
	if [ $pcm_have = 1 ] && pcm_okname "$pcm_n"; then
		pcm_get "$pcm_n"
		eval "pcm_prev_$pcm_n=\$pcm_gv"
		pcm_ol=$(command alias "$pcm_n")
		eval "pcm_oldl_$pcm_n=\$pcm_ol"
		pcm_redefs="$pcm_redefs $pcm_n"
	fi
	if [ "${PCM_FAULT-}" = sub_leaks ]; then command printf '%s\n' "$1" >>"$pcm_store"; fi
	case ${PCM_FAULT-} in
	define_noop) return 0 ;;
	define_status1) command alias "$1"; return 1 ;;
	define_first_word) pcm_v=${pcm_v%%[ $pcm_tab]*} ;;
	define_drop_last) pcm_v=${pcm_v%?} ;;
	define_wrong_name) pcm_n=${pcm_n%?} ;;
	define_split_last_eq) pcm_n=${1%=*}; pcm_v=${1##*=} ;;
	define_empty_ignored) if [ -z "$pcm_v" ]; then return 0; fi ;;
	trailing_blank_trimmed)
		while :; do
			case $pcm_v in
			*" " | *"$pcm_tab") pcm_v=${pcm_v%?} ;;
			*) break ;;
			esac
		done ;;
	trailing_blank_always) pcm_v="$pcm_v " ;;
	trailing_tab_ignored) pcm_v=${pcm_v%"$pcm_tab"} ;;
	recursion_twice)
		case $pcm_v in
		"$pcm_n" | "$pcm_n "*) pcm_v="$pcm_v${pcm_v#"$pcm_n"}" ;;
		esac ;;
	redefine_ignored) if [ $pcm_have = 1 ]; then return 0; fi ;;
	redefine_append) if [ $pcm_have = 1 ]; then pcm_get "$pcm_n"; pcm_v="$pcm_gv $pcm_v"; fi ;;
	redefine_exit1) if [ $pcm_have = 1 ]; then command alias "$pcm_n=$pcm_v"; return 1; fi ;;
	redefine_subst_old)
		if [ $pcm_have = 1 ] && pcm_okname "$pcm_n"; then
			eval "pcm_shown_$pcm_n=\$pcm_v"
			return 0
		fi ;;
	redefine_shows_old)
		if [ $pcm_have = 1 ] && pcm_okname "$pcm_n"; then
			eval "pcm_shown_$pcm_n=\$pcm_gv"
		fi ;;
	esac
	command alias "$pcm_n=$pcm_v"
}

pcm_missing() { # name index total
	case ${PCM_FAULT-} in
	missing_status0) command alias "$1"; return 0 ;;
	missing_stdout_diag) command alias "$1" 2>&1; return 1 ;;
	missing_nodiag) command alias "$1" 2>/dev/null; return 1 ;;
	missing_nodiag_multi)
		if [ "$3" -gt 1 ]; then command alias "$1" 2>/dev/null; return 1; fi ;;
	missing_empty_stdout) command printf '\n'; command alias "$1"; return $? ;;
	missing_prints_def) command printf "%s=''\n" "$1"; command alias "$1"; return 1 ;;
	missing_prefix)
		pcm_all=$(command alias)
		while IFS= read -r pcm_l; do
			case $pcm_l in
			"$1"*=*) command printf '%s\n' "$pcm_l"; return 0 ;;
			esac
		done <<PCM_EOF
$pcm_all
PCM_EOF
		;;
	missing_case_fold)
		pcm_lc=$(command printf '%s' "$1" | command tr 'A-Z' 'a-z')
		if [ "$pcm_lc" != "$1" ] && command alias "$pcm_lc" 2>/dev/null; then return 0; fi ;;
	esac
	command alias "$1"
}

pcm_show() { # name index total
	pcm_sn=$1
	if ! pcm_get "$pcm_sn"; then pcm_missing "$@"; return $?; fi
	pcm_sv=$pcm_gv
	pcm_has= pcm_sh= pcm_ol=
	if pcm_okname "$pcm_sn"; then
		eval "pcm_has=\${pcm_shown_$pcm_sn+x} pcm_sh=\${pcm_shown_$pcm_sn-} pcm_ol=\${pcm_oldl_$pcm_sn-}"
	fi
	case ${PCM_FAULT-} in
	quote_double) pcm_emit "$pcm_sn" "$pcm_sv" dq; return 0 ;;
	quote_backslash) pcm_emit "$pcm_sn" "$pcm_sv" bq; return 0 ;;
	show_no_newline) pcm_o=$(command alias "$pcm_sn"); command printf '%s' "$pcm_o"; return 0 ;;
	show_extra_blank) command alias "$pcm_sn"; command printf '\n'; return 0 ;;
	show_no_name) pcm_sq "$pcm_sv"; command printf '%s\n' "$pcm_q"; return 0 ;;
	show_space_sep) pcm_sq "$pcm_sv"; command printf '%s %s\n' "$pcm_sn" "$pcm_q"; return 0 ;;
	show_unquoted) command printf '%s=%s\n' "$pcm_sn" "$pcm_sv"; return 0 ;;
	show_sq_unescaped) command printf "%s='%s'\\n" "$pcm_sn" "$pcm_sv"; return 0 ;;
	show_dq_unescaped) command printf '%s="%s"\n' "$pcm_sn" "$pcm_sv"; return 0 ;;
	show_drop_backslash) pcm_del '\' "$pcm_sv"; pcm_emit "$pcm_sn" "$pcm_dres" sq; return 0 ;;
	show_trim_blank)
		pcm_t=$pcm_sv
		while :; do
			case $pcm_t in
			" "*) pcm_t=${pcm_t#?} ;;
			*" ") pcm_t=${pcm_t%?} ;;
			*) break ;;
			esac
		done
		pcm_emit "$pcm_sn" "$pcm_t" sq; return 0 ;;
	show_newline_to_space) pcm_repl "$pcm_nl" ' ' "$pcm_sv"; pcm_emit "$pcm_sn" "$pcm_dres" sq; return 0 ;;
	show_drop_last_char) pcm_emit "$pcm_sn" "${pcm_sv%?}" sq; return 0 ;;
	show_empty_value) command printf "%s=''\\n" "$pcm_sn"; return 0 ;;
	show_stderr) command alias "$pcm_sn" >&2; return 0 ;;
	show_all) command alias; return 0 ;;
	show_prefix)
		pcm_all=$(command alias)
		while IFS= read -r pcm_l; do
			case $pcm_l in
			"$pcm_sn"*=*) command printf '%s\n' "$pcm_l" ;;
			esac
		done <<PCM_EOF
$pcm_all
PCM_EOF
		return 0 ;;
	show_exit1) command alias "$pcm_sn"; return 1 ;;
	redefine_subst_old | redefine_shows_old)
		if [ -n "$pcm_has" ]; then pcm_emit "$pcm_sn" "$pcm_sh" sq; return 0; fi ;;
	redefine_shows_both)
		if [ -n "$pcm_ol" ]; then command printf '%s\n' "$pcm_ol"; fi ;;
	esac
	command alias "$pcm_sn"
}

pcm_list() {
	case ${PCM_FAULT-} in
	list_first_only) command alias | sed -n 1p; return 0 ;;
	list_last_only) command alias | sed -n '$p'; return 0 ;;
	list_skip_first) command alias | sed 1d; return 0 ;;
	list_skip_last) command alias | sed '$d'; return 0 ;;
	list_no_value) command alias | sed 's/=.*//'; return 0 ;;
	list_exit1) command alias; return 1 ;;
	list_stderr) command alias >&2; return 0 ;;
	list_empty_exit1)
		pcm_lo=$(command alias)
		if [ -z "$pcm_lo" ]; then return 1; fi
		command printf '%s\n' "$pcm_lo"; return 0 ;;
	list_no_newline) pcm_lo=$(command alias); command printf '%s' "$pcm_lo"; return 0 ;;
	list_reverse)
		pcm_lo=$(command alias)
		if [ -z "$pcm_lo" ]; then return 0; fi
		pcm_rl=
		while IFS= read -r pcm_ll; do
			pcm_rl=$pcm_ll$pcm_nl$pcm_rl
		done <<PCM_EOF
$pcm_lo
PCM_EOF
		command printf '%s' "$pcm_rl"; return 0 ;;
	quote_double | quote_backslash)
		pcm_lo=$(command alias)
		if [ -z "$pcm_lo" ]; then return 0; fi
		while IFS= read -r pcm_ll; do
			pcm_ln=${pcm_ll%%=*}
			if pcm_get "$pcm_ln"; then
				if [ "$PCM_FAULT" = quote_double ]; then pcm_emit "$pcm_ln" "$pcm_gv" dq
				else pcm_emit "$pcm_ln" "$pcm_gv" bq; fi
			else
				command printf '%s\n' "$pcm_ll"
			fi
		done <<PCM_EOF
$pcm_lo
PCM_EOF
		return 0 ;;
	list_old_kept)
		command alias
		for pcm_rn in $pcm_redefs; do
			eval "pcm_ol=\${pcm_oldl_$pcm_rn-}"
			if [ -n "$pcm_ol" ]; then command printf '%s\n' "$pcm_ol"; fi
		done
		return 0 ;;
	esac
	command alias
}

alias() {
	pcm_sync
	if [ $# -eq 0 ]; then pcm_list; return $?; fi
	pcm_hasdef=0 pcm_hasshow=0 pcm_total=$#
	for pcm_x do
		case $pcm_x in
		*=*) pcm_hasdef=1 ;;
		*) pcm_hasshow=1 ;;
		esac
	done
	if [ "${PCM_FAULT-}" = multi_show_reverse ] && [ $pcm_hasdef = 0 ]; then
		pcm_rev=
		for pcm_x do pcm_rev="$pcm_x $pcm_rev"; done
		# shellcheck disable=SC2086
		set -- $pcm_rev
	fi
	pcm_st=0 pcm_i=0
	for pcm_a do
		pcm_i=$((pcm_i + 1))
		pcm_skip=0
		case $pcm_a in
		*=*)
			case ${PCM_FAULT-} in
			multi_define_first_only) if [ $pcm_i -gt 1 ]; then pcm_skip=1; fi ;;
			multi_define_last_only) if [ $pcm_i -lt $pcm_total ]; then pcm_skip=1; fi ;;
			multi_mixed_no_define) if [ $pcm_hasshow = 1 ]; then pcm_skip=1; fi ;;
			esac
			if [ $pcm_skip = 1 ]; then pcm_r=0; else pcm_def "$pcm_a"; pcm_r=$?; fi ;;
		*)
			case ${PCM_FAULT-} in
			multi_show_first_only) if [ $pcm_i -gt 1 ]; then pcm_skip=1; fi ;;
			multi_show_last_only) if [ $pcm_i -lt $pcm_total ]; then pcm_skip=1; fi ;;
			multi_mixed_no_show) if [ $pcm_hasdef = 1 ]; then pcm_skip=1; fi ;;
			esac
			if [ $pcm_skip = 1 ]; then pcm_r=0; else pcm_show "$pcm_a" "$pcm_i" "$pcm_total"; pcm_r=$?; fi ;;
		esac
		case ${PCM_FAULT-} in
		missing_status_last) pcm_st=$pcm_r ;;
		missing_status_first) if [ $pcm_i -eq 1 ]; then pcm_st=$pcm_r; fi ;;
		missing_status2) if [ $pcm_r -ne 0 ]; then pcm_st=2; fi ;;
		missing_status255) if [ $pcm_r -ne 0 ]; then pcm_st=255; fi ;;
		*) if [ $pcm_r -ne 0 ]; then pcm_st=$pcm_r; fi ;;
		esac
	done
	return $pcm_st
}

eval() {
	case ${PCM_FAULT-} in
	sub_leaks) pcm_sync ;;
	sub_no_inherit) if pcm_insub; then command unalias -a; fi ;;
	define_once) command eval "$@"; pcm_er=$?; command unalias -a; return $pcm_er ;;
	quoted_bs)
		case $1 in
		'\'*) pcm_ew=${1#?}; shift; set -- "$pcm_ew" "$@" ;;
		esac ;;
	quoted_sq)
		case $1 in
		\'*\') pcm_ew=${1#?}; pcm_ew=${pcm_ew%?}; shift; set -- "$pcm_ew" "$@" ;;
		esac ;;
	quoted_dq)
		case $1 in
		\"*\") pcm_ew=${1#?}; pcm_ew=${pcm_ew%?}; shift; set -- "$pcm_ew" "$@" ;;
		esac ;;
	quoted_part_bs)
		case $1 in
		*'\'*) pcm_del '\' "$1"; shift; set -- "$pcm_dres" "$@" ;;
		esac ;;
	quoted_part_dq)
		case $1 in
		*'"'*) pcm_del '"' "$1"; shift; set -- "$pcm_dres" "$@" ;;
		esac ;;
	quoted_part_sq)
		case $1 in
		*"'"*) pcm_del "'" "$1"; shift; set -- "$pcm_dres" "$@" ;;
		esac ;;
	esac
	command eval "$@"
}

case ${PCM_FAULT-} in
inherit_in_child) PC_SH=pcm_shw ;;
list_predefined) command alias pcpre1='true' r='fc -s' ;;
esac

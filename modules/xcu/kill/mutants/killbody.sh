# The planted kill: one function, fault chosen by $PC_KM_FAULT. It does the
# real work with the shell's or the system's own kill and alters one thing.
# The same text is loaded as a shell function and, as a program, put first
# on PATH. It signals only the operands it is given.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
pc_km_rej() {
	echo "kill: bad operand" >&2
	return 2
}
pc_km_list() { # kill -l [operand]
	if [ $# -eq 0 ]; then
		pc_km_all=$(command kill -l)
		case $PC_KM_FAULT in
		list_sig_prefix) for pc_km_t in $pc_km_all; do printf 'SIG%s\n' "$pc_km_t"; done ;;
		list_lower) printf '%s\n' "$pc_km_all" | tr 'A-Z' 'a-z' ;;
		list_no_newline) printf '%s ' $pc_km_all ;;
		list_comma) printf '%s,' $pc_km_all ;;
		list_no_hup | list_no_kill | list_no_term | list_no_tstp)
			pc_km_x=${PC_KM_FAULT#list_no_}
			pc_km_x=$(printf '%s' "$pc_km_x" | tr 'a-z' 'A-Z')
			for pc_km_t in $pc_km_all; do [ "$pc_km_t" = "$pc_km_x" ] || printf '%s\n' "$pc_km_t"; done ;;
		list_stderr) printf '%s\n' "$pc_km_all" >&2 ;;
		list_status1) printf '%s\n' "$pc_km_all"; return 1 ;;
		list_empty) : ;;
		v_list_horizontal) pc_km_sp=
			for pc_km_t in $pc_km_all; do printf '%s%s' "$pc_km_sp" "$pc_km_t"; pc_km_sp=' '; done
			printf '\n' ;;
		v_list_vertical) for pc_km_t in $pc_km_all; do printf '%s\n' "$pc_km_t"; done ;;
		v_list_mixed) pc_km_i=0
			for pc_km_t in $pc_km_all; do
				pc_km_i=$((pc_km_i + 1))
				if [ $((pc_km_i % 5)) -eq 0 ]; then printf '%s\n' "$pc_km_t"; else printf '%s ' "$pc_km_t"; fi
			done
			printf '\n' ;;
		v_list_extra) printf '%s\n' "$pc_km_all"; printf 'XTRA1 XTRA2\n' ;;
		v_list_reverse) pc_km_rev=
			for pc_km_t in $pc_km_all; do pc_km_rev="$pc_km_t $pc_km_rev"; done
			pc_km_sp=
			for pc_km_t in $pc_km_rev; do printf '%s%s' "$pc_km_sp" "$pc_km_t"; pc_km_sp=' '; done
			printf '\n' ;;
		*) command kill -l ;;
		esac
		return
	fi
	pc_km_arg=$1
	pc_km_real=$(command kill -l "$pc_km_arg")
	pc_km_rc=$?
	case $PC_KM_FAULT in
	lop_prefix) printf 'SIG%s\n' "$pc_km_real" ;;
	lop_lower) printf '%s\n' "$pc_km_real" | tr 'A-Z' 'a-z' ;;
	lop_no_newline) printf '%s' "$pc_km_real" ;;
	lop_stderr) printf '%s\n' "$pc_km_real" >&2 ;;
	lop_all) command kill -l ;;
	lop_status1) printf '%s\n' "$pc_km_real"; return 1 ;;
	lop_blank_line) printf '%s\n\n' "$pc_km_real" ;;
	lop_off1) pc_km_n=$pc_km_arg
		[ "$pc_km_n" -gt 128 ] && pc_km_n=$((pc_km_n - 128))
		printf '%s\n' "$(command kill -l $((pc_km_n + 1)))" ;;
	lop_swap) case $pc_km_real in
		ALRM) pc_km_real=TERM ;;
		TERM) pc_km_real=ALRM ;;
		esac
		printf '%s\n' "$pc_km_real" ;;
	lop_status_unsupported) if [ "$pc_km_arg" -gt 128 ]; then
			echo "kill: unknown signal" >&2
			return 1
		fi
		printf '%s\n' "$pc_km_real" ;;
	lop_number_unsupported) if [ "$pc_km_arg" -le 128 ]; then
			echo "kill: unknown signal" >&2
			return 1
		fi
		printf '%s\n' "$pc_km_real" ;;
	*) printf '%s\n' "$pc_km_real"; return $pc_km_rc ;;
	esac
}
kill() {
	if [ "$1" = -l ]; then
		shift
		pc_km_list "$@"
		return
	fi
	case $PC_KM_FAULT in
	exec_drops) return 0 ;;
	exec_exit1) return 1 ;;
	esac
	pc_km_mode=default
	pc_km_sig=
	case $1 in
	-s) pc_km_mode=s pc_km_sig=$2; shift 2 ;;
	-[0-9]*) pc_km_mode=num pc_km_sig=${1#-}; shift ;;
	-[A-Za-z]*) pc_km_mode=name pc_km_sig=${1#-}; shift ;;
	esac
	pc_km_dd=
	if [ "$1" = -- ]; then pc_km_dd=--; shift; fi
	# faults on the signal chosen
	case $PC_KM_FAULT:$pc_km_mode in
	default_usr1:default) pc_km_mode=s pc_km_sig=USR1 ;;
	default_kill:default) pc_km_mode=s pc_km_sig=KILL ;;
	default_drops:default) return 0 ;;
	s_ignored:s) pc_km_mode=default ;;
	s_lower_rejected:s) case $pc_km_sig in *[a-z]*) pc_km_rej; return ;; esac ;;
	s_upper_rejected:s) case $pc_km_sig in *[A-Z]*) pc_km_rej; return ;; esac ;;
	s_mixed_rejected:s) case $pc_km_sig in *[a-z]*) case $pc_km_sig in *[A-Z]*) pc_km_rej; return ;; esac ;; esac ;;
	s_needs_prefix:s) case $pc_km_sig in SIG* | sig* | 0) ;; *) pc_km_rej; return ;; esac ;;
	s_usr1_as_usr2:s) case $pc_km_sig in [Uu][Ss][Rr]1) pc_km_sig=USR2 ;; esac ;;
	zero_sends_term:s) [ "$pc_km_sig" = 0 ] && pc_km_mode=default ;;
	zero_rejected:s) [ "$pc_km_sig" = 0 ] && { pc_km_rej; return; } ;;
	dash_name_ignored:name) pc_km_mode=default ;;
	dash_name_rejected:name) pc_km_rej; return ;;
	dash_name_upper_only:name) case $pc_km_sig in *[a-z]*) pc_km_rej; return ;; esac ;;
	dash_name_lower_only:name) case $pc_km_sig in *[A-Z]*) pc_km_rej; return ;; esac ;;
	dash_name_mixed_rejected:name) case $pc_km_sig in *[a-z]*) case $pc_km_sig in *[A-Z]*) pc_km_rej; return ;; esac ;; esac ;;
	dash_name_needs_prefix:name) case $pc_km_sig in SIG* | sig*) ;; *) pc_km_rej; return ;; esac ;;
	dash_num_rejected:num) pc_km_rej; return ;;
	dash_num_9_term:num) [ "$pc_km_sig" = 9 ] && pc_km_mode=default ;;
	dash_num_15_kill:num) [ "$pc_km_sig" = 15 ] && pc_km_sig=9 ;;
	dash_num_zero_term:num) [ "$pc_km_sig" = 0 ] && pc_km_mode=default ;;
	dash_num_14_term:num) [ "$pc_km_sig" = 14 ] && pc_km_sig=15 ;;
	esac
	# faults on the operands
	case $PC_KM_FAULT in
	only_first) [ $# -gt 1 ] && set -- "$1" ;;
	only_last) [ $# -gt 1 ] && shift $(($# - 1)) ;;
	pid0_drops) [ "$1" = 0 ] && return 0 ;;
	pid0_rejected) [ "$1" = 0 ] && { pc_km_rej; return; } ;;
	pid0_exit1) [ "$1" = 0 ] && return 1 ;;
	neg_as_positive) case $1 in -[0-9]*) set -- "${1#-}" ;; esac ;;
	neg_drops) case $1 in -[0-9]*) return 0 ;; esac ;;
	neg_rejected) case $1 in -[0-9]*) pc_km_rej; return ;; esac ;;
	dd_rejected) [ -n "$pc_km_dd" ] && { pc_km_rej; return; } ;;
	job_rejected) case $1 in %*) return 1 ;; esac ;;
	job_drops) case $1 in %*) return 0 ;; esac ;;
	job_default_usr1) case $1 in %*) [ $pc_km_mode = default ] && { pc_km_mode=s; pc_km_sig=USR1; } ;; esac ;;
	job_n_shift) case $1 in %[0-9]*) set -- "%$((${1#%} + 1))" ;; esac ;;
	esac
	# send
	case $PC_KM_FAULT in
	multi_status_first | multi_status_last)
		pc_km_first= pc_km_last=
		pc_km_cnt=0
		for pc_km_op in "$@"; do
			pc_km_do "$pc_km_op"
			pc_km_last=$?
			[ $pc_km_cnt -eq 0 ] && pc_km_first=$pc_km_last
			pc_km_cnt=$((pc_km_cnt + 1))
		done
		if [ "$PC_KM_FAULT" = multi_status_first ]; then return "$pc_km_first"; fi
		return "$pc_km_last" ;;
	esac
	pc_km_do "$@"
	pc_km_rc=$?
	case $PC_KM_FAULT in
	gone_ok) return 0 ;;
	zero_gone_ok) [ "$pc_km_mode:$pc_km_sig" = s:0 ] && return 0 ;;
	gone_stdout) [ $pc_km_rc -ne 0 ] && echo "kill: no such process" ;;
	send_stdout) [ $pc_km_rc -eq 0 ] && echo "sent" ;;
	send_status1) [ $pc_km_rc -eq 0 ] && return 1 ;;
	exec_stdout) [ $pc_km_rc -eq 0 ] && echo "sent" ;;
	exec_zero_exit1) [ "$pc_km_sig" = 0 ] && return 1 ;;
	err_status77) [ $pc_km_rc -ne 0 ] && return 77 ;;
	esac
	return $pc_km_rc
}
pc_km_do() { # the real send with the (possibly altered) signal and the operands
	case $pc_km_mode in
	default) command kill ${pc_km_dd:+--} "$@" 2>/dev/null ;;
	s | name) command kill -s "$pc_km_sig" ${pc_km_dd:+--} "$@" 2>/dev/null ;;
	num) command kill "-$pc_km_sig" ${pc_km_dd:+--} "$@" 2>/dev/null ;;
	esac
}

# Shared by the getopts faults and variants: a small reimplementation of
# getopts, written in the shell itself, with one planted fault or variant
# chosen by the name in pcm_fault (none behaves correctly).
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# Usage, from a fault or a variant file:
#     . "${PC_MUTANT_FILE%/*}/gorref.sh"     (a variant: .../../mutants/gorref.sh)
#     pcm_fault=NAME
#     pcm_install
#
# A shell function cannot see the positional parameters of the code that
# calls it, so pcm_install defines an alias that passes them on, in front
# of the arguments of the call. The reference then behaves as the utility
# does for the parameters the call names, or for those it was handed.
#
# The state of a parse lives in OPTIND alone, as a number: below 10000 it
# is the index of the next word to look at; above, it also encodes how many
# characters of the word at that index were already used (the text leaves
# OPTIND unspecified except after an option with an argument and at the end
# of the options). With the variant optind_opaque the value after a
# successful call that is not an option with an argument is encoded too.

pcm_fault=
pcm_mem=
pcm_memind=
pcm_mempos=

pcm_is() { [ "$pcm_fault" = "$1" ]; }

pcm_assign() { eval "$1=\$2"; }

# can the variable be given a value (is it not read-only)?
pcm_can() { (eval "$1=") 2>/dev/null; }

# the diagnostic: pcm_diag KIND CHAR (KIND u: unknown option, m: missing argument)
pcm_diag() {
	pcm_p=$0
	case $pcm_fault in
	diag_fullpath) ;;
	*) pcm_p=${0##*/} ;;
	esac
	if pcm_is diag_no_prog || { pcm_is diag_missing_no_prog && [ "$1" = m ]; }; then pcm_p=; fi
	if pcm_is diag_prog_getopts; then pcm_p=getopts; fi
	pcm_ch=$2
	if { pcm_is diag_unknown_no_opt && [ "$1" = u ]; } || { pcm_is diag_missing_no_opt && [ "$1" = m ]; }; then pcm_ch=; fi
	if pcm_is diag_usage; then
		if [ "$1" = u ]; then pcm_msg="usage error ($pcm_p): bad option -$pcm_ch"
		else pcm_msg="usage error ($pcm_p): no argument given to -$pcm_ch"; fi
	elif [ "$1" = u ]; then pcm_msg="$pcm_p: illegal option -- $pcm_ch"
	else pcm_msg="$pcm_p: option requires an argument -- $pcm_ch"
	fi
	if pcm_is diag_stdout; then
		command printf '%s\n' "$pcm_msg"
	elif pcm_is diag_both; then
		command printf '%s\n' "$pcm_msg"
		command printf '%s\n' "$pcm_msg" >&2
	else
		command printf '%s\n' "$pcm_msg" >&2
	fi
}

# remember where this call left the parse (index and characters used)
pcm_save() {
	pcm_memind=$pcm_ind
	pcm_mempos=$pcm_pos
	if [ -n "$pcm_rest" ]; then
		OPTIND=$((pcm_ind + 10000 * pcm_pos))
	elif pcm_is optind_opaque; then
		OPTIND=$((pcm_ind + 1 + 100000000))
	else
		OPTIND=$((pcm_ind + 1))
	fi
	pcm_mem=$OPTIND
	if pcm_is restart_each_call; then OPTIND=1; fi
}

# the end of the options; pcm_end INDEX KIND
pcm_end() {
	pcm_v=$1
	case $2 in
	ddash)
		pcm_is ddash_optind_at && pcm_v=$((pcm_v - 1))
		pcm_is ddash_optind_plus1 && pcm_v=$((pcm_v + 1))
		;;
	operand)
		pcm_is operand_optind_after && pcm_v=$((pcm_v + 1))
		;;
	esac
	pcm_is end_optind_minus1 && pcm_v=$((pcm_v - 1))
	pcm_is end_optind_plus1 && pcm_v=$((pcm_v + 1))
	pcm_is end_noperand_clamp && [ "$pcm_v" -gt "$pcm_np" ] && [ "$pcm_np" -gt 0 ] && pcm_v=$pcm_np
	pcm_is param_end_minus1 && [ "$pcm_haveparams" = 1 ] && pcm_v=$((pcm_v - 1))
	if pcm_is end_name_kept || { pcm_is ddash_name_kept && [ "$2" = ddash ]; } || { pcm_is operand_name_kept && [ "$2" = operand ]; }; then
		:
	elif pcm_is end_name_empty; then
		pcm_assign "$pcm_nm" ""
	else
		pcm_assign "$pcm_nm" '?'
	fi
	OPTIND=$pcm_v
	if pcm_is end_optarg_stale; then :
	elif pcm_is end_optarg_empty; then OPTARG=
	else unset OPTARG; fi
	pcm_mem=$OPTIND
	pcm_memind=$pcm_v
	pcm_mempos=0
	pcm_rc=1
	pcm_is end_status0 && pcm_rc=0
	pcm_is end_status2 && pcm_rc=2
	pcm_is ddash_status0 && [ "$2" = ddash ] && pcm_rc=0
	pcm_is operand_status0 && [ "$2" = operand ] && pcm_rc=0
	return "$pcm_rc"
}

# the work: parameters in "$@", optstring in pcm_os, name in pcm_nm
pcm_step() {
	pcm_np=$#
	pcm_errst=2
	pcm_is readonly_status1 && pcm_errst=1
	pcm_is readonly_status0 && pcm_errst=0
	pcm_is status255 && pcm_errst=255
	pcm_can "$pcm_nm" || return "$pcm_errst"
	if ! pcm_can OPTARG; then
		pcm_is readonly_optarg_status0 && return 0
		return "$pcm_errst"
	fi
	if ! pcm_can OPTIND; then
		pcm_is readonly_optind_status0 && return 0
		return "$pcm_errst"
	fi

	# where the parse stands
	pcm_ostate=${OPTIND-}
	case $pcm_ostate in ''|*[!0-9]*) pcm_ostate=1 ;; esac
	if pcm_is reset_ignored && [ -n "$pcm_mem" ]; then pcm_ostate=$pcm_mem; fi
	pcm_ind=$((pcm_ostate % 10000))
	pcm_pos=$(( (pcm_ostate / 10000) % 10000 ))
	if pcm_is reset_keeps_cluster && [ "$pcm_ostate" = 1 ] && [ "${pcm_mempos:-0}" -gt 0 ]; then
		pcm_ind=$pcm_memind
		pcm_pos=$pcm_mempos
	fi
	[ "$pcm_ind" -ge 1 ] || pcm_ind=1
	pcm_rest=
	if [ "$pcm_pos" -gt 0 ] && [ "$pcm_ind" -le "$pcm_np" ]; then
		eval "pcm_w=\${$pcm_ind}"
		case $pcm_w in
		-?*)
			pcm_rest=${pcm_w#-}
			pcm_i=0
			while [ "$pcm_i" -lt "$pcm_pos" ]; do pcm_rest=${pcm_rest#?}; pcm_i=$((pcm_i + 1)); done
			;;
		esac
	fi
	[ -n "$pcm_rest" ] || pcm_pos=0

	# take a new word when none is under way
	pcm_first=
	while [ -z "$pcm_rest" ]; do
		if [ "$pcm_ind" -gt "$pcm_np" ]; then pcm_end "$pcm_ind" none; return $?; fi
		eval "pcm_w=\${$pcm_ind}"
		case $pcm_w in
		--)
			if pcm_is ddash_continue; then pcm_ind=$((pcm_ind + 1)); continue; fi
			pcm_end $((pcm_ind + 1)) ddash
			return $?
			;;
		-?*)
			pcm_rest=${pcm_w#-}
			pcm_pos=0
			pcm_first=1
			;;
		*)
			if pcm_is operand_continue; then pcm_ind=$((pcm_ind + 1)); continue; fi
			pcm_end "$pcm_ind" operand
			return $?
			;;
		esac
	done

	# the next option character
	if pcm_is cluster_reverse; then
		pcm_c=${pcm_rest#"${pcm_rest%?}"}
		pcm_rest=${pcm_rest%?}
		pcm_pos=$((pcm_pos + 1))
	elif pcm_is cluster_whole && [ -n "${pcm_rest#?}" ]; then
		pcm_c=$pcm_rest
		pcm_rest=
		pcm_pos=$((pcm_pos + 1))
	else
		pcm_c=${pcm_rest%"${pcm_rest#?}"}
		pcm_rest=${pcm_rest#?}
		pcm_pos=$((pcm_pos + 1))
	fi
	pcm_colon=
	pcm_o=$pcm_os
	case $pcm_o in :*) pcm_colon=1; pcm_o=${pcm_o#:} ;; esac
	pcm_known=
	pcm_arg=
	if pcm_is cluster_whole && [ "${#pcm_c}" -gt 1 ]; then
		pcm_known=1
	else
		case $pcm_c in
		:|'?') ;;
		*)
			while [ -n "$pcm_o" ]; do
				pcm_oc=${pcm_o%"${pcm_o#?}"}
				pcm_o=${pcm_o#?}
				pcm_a=
				case $pcm_o in :*) pcm_a=1; pcm_o=${pcm_o#:} ;; esac
				if [ "$pcm_oc" = "$pcm_c" ]; then pcm_known=1; pcm_arg=$pcm_a; break; fi
			done
			;;
		esac
	fi

	# an option that is not in the optstring
	if [ -z "$pcm_known" ]; then
		pcm_rc=0
		if [ -n "$pcm_colon" ]; then
			if pcm_is ucolon_name_colon; then pcm_assign "$pcm_nm" ':'; else pcm_assign "$pcm_nm" '?'; fi
			if pcm_is ucolon_optarg_unset; then unset OPTARG
			elif pcm_is ucolon_optarg_q; then OPTARG='?'
			else OPTARG=$pcm_c; fi
			pcm_is ucolon_diag && pcm_diag u "$pcm_c"
			pcm_is ucolon_status1 && pcm_rc=1
		else
			if pcm_is unknown_name_opt; then pcm_assign "$pcm_nm" "$pcm_c"; else pcm_assign "$pcm_nm" '?'; fi
			if pcm_is unknown_optarg_set; then OPTARG=$pcm_c
			elif pcm_is unknown_optarg_stale; then :
			else unset OPTARG; fi
			pcm_is unknown_no_diag || pcm_diag u "$pcm_c"
			pcm_is unknown_status1 && pcm_rc=1
			pcm_is unknown_status2 && pcm_rc=2
		fi
		pcm_save
		return "$pcm_rc"
	fi

	# a known option that takes no argument
	if [ -n "$pcm_arg" ] && pcm_is cluster_attached_arg_ignored && [ "$pcm_pos" -gt 1 ] && [ -n "$pcm_rest" ]; then
		pcm_arg=
	fi
	if [ -z "$pcm_arg" ]; then
		if pcm_is flag_name_empty; then pcm_assign "$pcm_nm" ""; else pcm_assign "$pcm_nm" "$pcm_c"; fi
		if pcm_is flag_optarg_stale; then :
		elif pcm_is flag_optarg_empty; then OPTARG=
		else unset OPTARG; fi
		if pcm_is cluster_skip_rest && [ -n "$pcm_first" ] && [ -n "$pcm_rest" ]; then pcm_rest=; fi
		pcm_save
		pcm_rc=0
		pcm_is flag_status1 && pcm_rc=1
		return "$pcm_rc"
	fi

	# a known option that takes an argument
	pcm_missing=
	if [ -n "$pcm_rest" ]; then
		pcm_how=att
		pcm_val=$pcm_rest
		pcm_rest=
		pcm_next=$((pcm_ind + 1))
		pcm_is att_optarg_letter && pcm_val=$pcm_c$pcm_val
		pcm_is att_optarg_empty && pcm_val=
		pcm_is optarg_first_char && pcm_val=${pcm_val%"${pcm_val#?}"}
		pcm_is att_optind_minus1 && pcm_next=$((pcm_next - 1))
		pcm_is att_optind_plus1 && pcm_next=$((pcm_next + 1))
	elif [ $((pcm_ind + 1)) -le "$pcm_np" ]; then
		pcm_how=sep
		eval "pcm_val=\${$((pcm_ind + 1))}"
		pcm_next=$((pcm_ind + 2))
		pcm_is sep_optarg_empty && pcm_val=
		pcm_is optarg_firstword && pcm_val=${pcm_val%% *}
		pcm_is sep_optind_minus1 && pcm_next=$((pcm_next - 1))
		pcm_is sep_optind_plus1 && pcm_next=$((pcm_next + 1))
		if pcm_is empty_arg_missing && [ -z "$pcm_val" ]; then pcm_missing=1; fi
		if pcm_is ddash_arg_missing && [ "$pcm_val" = -- ]; then pcm_missing=1; fi
	else
		pcm_missing=1
	fi
	if [ -z "$pcm_missing" ]; then
		if pcm_is arg_name_empty; then pcm_assign "$pcm_nm" ""; else pcm_assign "$pcm_nm" "$pcm_c"; fi
		if [ "$pcm_how" = sep ] && pcm_is sep_optarg_unset; then unset OPTARG; else OPTARG=$pcm_val; fi
		OPTIND=$pcm_next
		pcm_mem=$OPTIND
		pcm_memind=$pcm_next
		pcm_mempos=0
		pcm_rest=
		pcm_rc=0
		pcm_is arg_status1 && pcm_rc=1
		return "$pcm_rc"
	fi

	# the option-argument is missing
	pcm_rc=0
	if [ -n "$pcm_colon" ]; then
		if pcm_is mcolon_name_q; then pcm_assign "$pcm_nm" '?'
		elif pcm_is mcolon_name_opt; then pcm_assign "$pcm_nm" "$pcm_c"
		else pcm_assign "$pcm_nm" ':'; fi
		if pcm_is mcolon_optarg_unset; then unset OPTARG
		elif pcm_is mcolon_optarg_colon; then OPTARG=':'
		else OPTARG=$pcm_c; fi
		pcm_is mcolon_diag && pcm_diag m "$pcm_c"
		pcm_is mcolon_status1 && pcm_rc=1
	else
		if pcm_is missing_name_colon; then pcm_assign "$pcm_nm" ':'
		elif pcm_is missing_name_opt; then pcm_assign "$pcm_nm" "$pcm_c"
		else pcm_assign "$pcm_nm" '?'; fi
		if pcm_is missing_optarg_set; then OPTARG=$pcm_c
		elif pcm_is missing_optarg_stale; then :
		else unset OPTARG; fi
		pcm_is missing_no_diag || pcm_diag m "$pcm_c"
		pcm_is missing_status1 && pcm_rc=1
		pcm_is missing_status2 && pcm_rc=2
	fi
	pcm_rest=
	pcm_save
	return "$pcm_rc"
}

# the entry point behind the alias: N, the caller's N positional parameters,
# the optstring, the name and the param operands (if any)
pcm_g() {
	pcm_n=$1
	shift
	pcm_k=$(($# - pcm_n - 2))
	pcm_haveparams=0
	if [ "$pcm_k" -le 0 ]; then
		pcm_i=0
		while [ "$pcm_i" -lt "$pcm_n" ]; do set -- "$@" "$1"; shift; pcm_i=$((pcm_i + 1)); done
		pcm_os=$1
		pcm_nm=$2
		shift 2
		if pcm_is positional_ignored; then set --; fi
	else
		pcm_haveparams=1
		if pcm_is param_ignored; then
			pcm_i=0
			while [ "$pcm_i" -lt "$pcm_n" ]; do set -- "$@" "$1"; shift; pcm_i=$((pcm_i + 1)); done
			pcm_os=$1
			pcm_nm=$2
			shift 2
			shift "$pcm_k"
		else
			shift "$pcm_n"
			pcm_os=$1
			pcm_nm=$2
			shift 2
		fi
	fi
	pcm_step "$@"
	pcm_r=$?
	pcm_is export_optind && export OPTIND
	if pcm_is export_optarg && [ "${OPTARG+set}" = set ]; then export OPTARG; fi
	return "$pcm_r"
}

pcm_install() {
	case $pcm_fault in
	optind_zero) OPTIND=0; return 0 ;;
	optind_unset) unset OPTIND; return 0 ;;
	esac
	shopt -s expand_aliases 2>/dev/null
	alias getopts='pcm_g "$#" "$@"'
}

# Shared by the mkdir faults: a thin layer over the real utility with one
# planted fault, chosen by name. It splits options itself and creates each
# operand with the real utility, so everything not named by the fault is the
# real behaviour.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# pc_mkdirref FAULT ARGS...
pc_mkdirref() (
	M=$1
	shift
	popt= mopt=
	OPTIND=1
	while getopts pm: o; do
		case $o in
		p) popt=1 ;;
		m) mopt=$OPTARG ;;
		*) exit 2 ;;
		esac
	done
	shift $((OPTIND - 1))
	mask=$(umask)
	mask=$((0$mask))
	rc=0 lastr=0 okl= i=0 n=$#
	[ "$M" = p_ignored ] && popt=
	[ "$M" = m_ignored ] && mopt=
	[ "$M" = p_m_ignored ] && [ -n "$popt" ] && mopt=
	oct() { printf '%o' "$1"; }
	run() {
		if [ "$M" = no_diag ]; then "$@" 2>/dev/null; else "$@"; fi
	}
	one() {
		d=$1
		if [ -z "$popt" ] && [ -d "$d" ]; then
			case $M in
			exists_ok) return 0 ;;
			exists_diag_ok)
				echo "mkdir: $d: exists" >&2
				return 0
				;;
			esac
		fi
		if [ -n "$popt" ]; then
			if [ -d "$d" ]; then
				case $M in
				p_exist_error)
					echo "mkdir: $d: exists" >&2
					return 1
					;;
				p_exist_chmod)
					chmod "$(oct $((0777 & ~mask)))" "$d"
					return 0
					;;
				p_exist_clobber) rm -rf "$d" ;;
				*) return 0 ;;
				esac
			fi
			if [ "$M" = p_file_last_ok ] && [ -e "$d" ]; then return 0; fi
			mid=$(((0300 | ~mask) & 0777))
			case $M in
			p_mid_plain) mid=$((0777 & ~mask)) ;;
			p_mid_777) mid=$((0777)) ;;
			p_mid_700) mid=$((0700)) ;;
			p_mid_no_w) mid=$(((0100 | ~mask) & 0777)) ;;
			p_mid_no_x) mid=$(((0200 | ~mask) & 0777)) ;;
			p_mid_gets_m)
				case $mopt in
				[0-7]*) mid=$((0$mopt)) ;;
				esac
				;;
			esac
			p=$d
			while :; do
				case $p in
				*/) p=${p%/} ;;
				*) break ;;
				esac
			done
			acc= rest=$p
			case $p in
			/*)
				acc=/
				rest=${p#/}
				;;
			esac
			made=0 swallow=
			while :; do
				case $rest in
				*/*) ;;
				*) break ;;
				esac
				comp=${rest%%/*}
				rest=${rest#*/}
				[ -n "$comp" ] || continue
				cur=$acc$comp
				acc=$cur/
				[ -d "$cur" ] && continue
				if [ -e "$cur" ]; then
					case $M in
					p_file_ok)
						swallow=1
						continue
						;;
					p_file_replaced) rm -f "$cur" ;;
					esac
				fi
				[ "$M" = p_one_level ] && [ $made -ge 1 ] && break
				run command mkdir -m 000 "$cur" || return 1
				chmod "$(oct "$mid")" "$cur" || return 1
				made=$((made + 1))
			done
			if [ -n "$swallow" ]; then
				command mkdir "$d" 2>/dev/null
				return 0
			fi
		fi
		fm=$mopt
		[ "$M" = p_last_ignores_m ] && [ -n "$popt" ] && fm=
		if [ -n "$fm" ]; then
			case $M:$fm in
			m_masked:[0-7]*)
				run command mkdir -m "$fm" "$d" &&
					chmod "$(oct $((0$fm & ~mask & 0777)))" "$d"
				;;
			m_drop_other:[0-7]*)
				run command mkdir -m "$fm" "$d" &&
					chmod "$(oct $((0$fm & 0770)))" "$d"
				;;
			m_sym_from_umask:*)
				run command mkdir "$d" && chmod "$fm" "$d"
				;;
			*) run command mkdir -m "$fm" "$d" ;;
			esac
		else
			case $M in
			default_777) run command mkdir "$d" && chmod 777 "$d" ;;
			default_755) run command mkdir "$d" && chmod 755 "$d" ;;
			default_666) run command mkdir -m "$(oct $((0666 & ~mask)))" "$d" ;;
			p_last_mid_mode)
				run command mkdir "$d" &&
					if [ -n "$popt" ]; then chmod "$(oct "$mid")" "$d"; fi
				;;
			*) run command mkdir "$d" ;;
			esac
		fi
		r=$?
		if [ $r -eq 0 ] && [ "$M" = stdout_noise ]; then echo "mkdir: created $d"; fi
		return $r
	}
	walk_rev() {
		if [ $# -gt 0 ]; then
			a=$1
			shift
			walk_rev "$@"
			step "$a"
		fi
	}
	step() {
		one "$1"
		r=$?
		lastr=$r
		if [ $r -ne 0 ]; then
			rc=1
			[ "$M" = stops_on_error ] && exit 1
		else
			okl="$okl $1"
		fi
	}
	if [ "$M" = reverse_order ]; then
		walk_rev "$@"
	else
		for d; do
			i=$((i + 1))
			[ "$M" = skip_last ] && [ $n -gt 1 ] && [ $i -eq $n ] && continue
			[ "$M" = p_first_only ] && [ $i -gt 1 ] && popt=
			step "$d"
		done
	fi
	if [ "$M" = rollback ] && [ $rc -ne 0 ]; then
		for d in $okl; do command rmdir "$d" 2>/dev/null; done
	fi
	[ "$M" = ok_status1 ] && exit 1
	[ "$M" = status0_on_error ] && exit 0
	[ "$M" = status_last_only ] && exit $lastr
	exit $rc
)

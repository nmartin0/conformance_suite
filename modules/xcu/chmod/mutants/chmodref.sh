# Shared by the chmod faults and variants: a small reimplementation of chmod
# in POSIX sh, with one planted fault or conforming choice picked by name.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# pc_chmodref NAME ARGS...: NAME "none" is the correct behaviour. It reads
# the current mode from the long listing, works the new mode out as a
# number, and hands that number to the real utility (called through
# "command", so a function of the same name is not reached).
#
# Working variables are shared on purpose (no "local" in POSIX sh): the
# recursion into a directory runs in a subshell, so nothing leaks.

pc_chmodref_diag() {
	case $M in
	err_no_diag) ;;
	err_diag_stdout) echo "chmod: cannot change $1" ;;
	err_diag_both)
		echo "chmod: cannot change $1"
		echo "chmod: cannot change $1" >&2
		;;
	*) echo "chmod: cannot change $1" >&2 ;;
	esac
}

# Reads the mode of $1 into cur (a number), isdir and isreg.
pc_chmodref_stat() {
	s=$(command ls -ld "$1" 2>/dev/null) || return 1
	s=${s%% *}
	isdir=0 isreg=0
	case $s in
	d*) isdir=1 ;;
	-*) isreg=1 ;;
	esac
	t=${s#?}
	i=0 cur=0
	while [ "$i" -lt 9 ]; do
		ch=${t%"${t#?}"}
		t=${t#?}
		bit=$((256 >> i))
		case $ch in
		r | w | x) cur=$((cur | bit)) ;;
		s | S)
			[ "$ch" = s ] && cur=$((cur | bit))
			if [ "$i" -eq 2 ]; then cur=$((cur | 04000)); else cur=$((cur | 02000)); fi
			;;
		t | T)
			[ "$ch" = t ] && cur=$((cur | bit))
			cur=$((cur | 01000))
			;;
		esac
		i=$((i + 1))
	done
	return 0
}

pc_chmodref_oct() {
	m=$((0$1))
	case $M in
	octal_umask) m=$((m & ~(um & 0777))) ;;
	octal_decimal)
		md=$1
		while :; do
			case $md in
			0?*) md=${md#0} ;;
			*) break ;;
			esac
		done
		m=$((md & 07777))
		;;
	octal_go_swapped) m=$(((m & 07700) | ((m & 070) >> 3) | ((m & 07) << 3))) ;;
	octal_special_dropped) m=$((m & 01777)) ;;
	octal_setgid_dropped) m=$((m & 05777)) ;;
	octal_sticky_dropped) m=$((m & 06777)) ;;
	esac
	new=$m
	case $M in
	octal_or) new=$((cur | m)) ;;
	octal_clear_only) new=$((cur & ~m & 07777)) ;;
	octal_special_kept) new=$((m | (cur & 06000))) ;;
	octal_setgid_kept) new=$((m | (cur & 02000))) ;;
	octal_sticky_kept) new=$((m | (cur & 01000))) ;;
	esac
}

# One action, such as "+rw", "g=u", "-X". Uses the class flags of the clause.
pc_chmodref_act() {
	op=${1%"${1#?}"}
	pl=${1#?}
	au=$tu ag=$tg ao=$to
	v=0 hasS=0 hasT=0 hasX=0
	copy=
	case $pl in
	[ugo]) copy=$pl ;;
	esac
	if [ -n "$copy" ]; then
		c=$copy
		if [ "$M" = copy_wrong_class ]; then
			case $c in
			u) c=g ;;
			g) c=o ;;
			o) c=u ;;
			esac
		fi
		case $c in
		u) sh=6 ;;
		g) sh=3 ;;
		o) sh=0 ;;
		esac
		src=$new
		[ "$M" = copy_original ] && src=$cur0
		v=$(((src >> sh) & 7))
		if [ "$M" = copy_plus_noop ] && [ "$op" = + ]; then return 0; fi
		if [ "$M" = copy_minus_noop ] && [ "$op" = - ]; then return 0; fi
	else
		x=$pl
		while [ -n "$x" ]; do
			c=${x%"${x#?}"}
			x=${x#?}
			case $c in
			r) v=$((v | 4)) ;;
			w) v=$((v | 2)) ;;
			x) v=$((v | 1)) ;;
			X) hasX=1 ;;
			s) hasS=1 ;;
			t) hasT=1 ;;
			esac
		done
	fi
	if [ "$hasX" = 1 ]; then
		xok=0
		[ "$isdir" = 1 ] && xok=1
		[ $((cur0 & 0111)) -ne 0 ] && xok=1
		case $M in
		X_always) xok=1 ;;
		X_never) xok=0 ;;
		X_dir_not)
			xok=0
			[ $((cur0 & 0111)) -ne 0 ] && xok=1
			;;
		X_owner_only)
			xok=0
			[ "$isdir" = 1 ] && xok=1
			[ $((cur0 & 0100)) -ne 0 ] && xok=1
			;;
		X_first_operand)
			if [ -z "$xfirst" ]; then xfirst=$xok; else xok=$xfirst; fi
			;;
		X_minus_noop) [ "$op" = - ] && xok=0 ;;
		X_eq_noop) [ "$op" = = ] && return 0 ;;
		esac
		[ "$xok" = 1 ] && v=$((v | 1))
		if [ "$M" = X_ignores_who ]; then au=1 ag=1 ao=1; fi
	fi
	case $M in
	s_ignored) hasS=0 ;;
	t_ignored) hasT=0 ;;
	t_nowho_ignored) [ "$nowho" = 1 ] && hasT=0 ;;
	t_a_ignored) [ "$nowho" = 0 ] && hasT=0 ;;
	t_minus_noop) [ "$op" = - ] && hasT=0 ;;
	s_minus_noop) [ "$op" = - ] && hasS=0 ;;
	esac
	spb=0
	if [ "$hasS" = 1 ]; then
		[ "$au" = 1 ] && spb=$((spb | 04000))
		[ "$ag" = 1 ] && spb=$((spb | 02000))
		if [ "$au" = 1 ] && [ "$ag" = 0 ]; then
			case $M in
			u_s_as_gid) spb=02000 ;;
			u_s_both) spb=06000 ;;
			u_minus_s_both) [ "$op" = - ] && spb=06000 ;;
			esac
		fi
		if [ "$ag" = 1 ] && [ "$au" = 0 ]; then
			case $M in
			g_s_as_uid) spb=04000 ;;
			g_s_both) spb=06000 ;;
			g_minus_s_noop) [ "$op" = - ] && spb=0 ;;
			esac
		fi
		if [ "$au" = 1 ] && [ "$ag" = 1 ] && [ "$nowho" = 0 ] && [ "$M" = a_s_only_u ]; then
			spb=04000
		fi
		if [ "$au" = 0 ] && [ "$ag" = 0 ] && [ "$ao" = 1 ]; then
			case $M in
			o_s_sets) spb=06000 ;;
			o_s_error)
				echo "chmod: the s permission needs a user or group class" >&2
				exit 1
				;;
			esac
		fi
	fi
	[ "$hasT" = 1 ] && spb=$((spb | 01000))
	b=0
	[ "$au" = 1 ] && b=$((b | (v << 6)))
	[ "$ag" = 1 ] && b=$((b | (v << 3)))
	[ "$ao" = 1 ] && b=$((b | v))
	b=$((b | spb))
	mk=0
	[ "$nowho" = 1 ] && mk=1
	[ "$M" = umask_with_who ] && mk=1
	[ "$M" = umask_ignored ] && mk=0
	case $M:$op in
	umask_not_plus:+ | umask_not_minus:- | umask_not_eq:=) mk=0 ;;
	esac
	if [ "$mk" = 1 ]; then
		u2=$um
		case $M in
		umask_user_part_ignored) u2=$((u2 & 077)) ;;
		umask_group_part_ignored) u2=$((u2 & 0707)) ;;
		umask_other_part_ignored) u2=$((u2 & 0770)) ;;
		esac
		if [ "$M" = umask_inverted ]; then
			b=$(((b & 07000) | (b & u2 & 0777)))
		else
			b=$(((b & 07000) | (b & ~u2 & 0777)))
		fi
	fi
	cm=0
	[ "$au" = 1 ] && cm=$((cm | 04700))
	[ "$ag" = 1 ] && cm=$((cm | 02070))
	[ "$ao" = 1 ] && cm=$((cm | 07))
	[ "$M" = plus_as_eq ] && [ "$op" = + ] && op==
	case $op in
	+)
		new=$((new | b))
		if [ "$M" = o_s_clears ] && [ "$hasS" = 1 ] && [ "$au" = 0 ] && [ "$ag" = 0 ]; then
			new=$((new & ~06000))
		fi
		;;
	-)
		cl2=$b
		[ "$M" = minus_noop ] && cl2=0
		[ "$M" = minus_clears_class ] && cl2=$cm
		new=$((new & ~cl2 & 07777))
		;;
	=)
		if [ "$M" = eq_empty_noop ] && [ -z "$pl" ]; then return 0; fi
		clr=$cm
		[ "$nowho" = 1 ] && clr=07777
		case $M in
		eq_clears_all) clr=07777 ;;
		eq_as_plus) clr=0 ;;
		eq_nowho_keeps) [ "$nowho" = 1 ] && clr=0 ;;
		eq_masked_clear) [ "$nowho" = 1 ] && clr=$((07777 & ~um)) ;;
		eq_empty_all) [ -z "$pl" ] && clr=07777 ;;
		esac
		new=$(((new & ~clr & 07777) | b))
		;;
	esac
	return 0
}

# One clause: a who list, then one or more actions.
pc_chmodref_clause() {
	cl=$1 w=
	while :; do
		case $cl in
		[ugoa]*)
			w=$w${cl%"${cl#?}"}
			cl=${cl#?}
			;;
		*) break ;;
		esac
	done
	nowho=0
	[ -z "$w" ] && nowho=1
	case $M in
	who_ignored) [ "$nowho" = 0 ] && w=a ;;
	wholist_first_only) w=${w%"${w#?}"} ;;
	nowho_as_user)
		if [ "$nowho" = 1 ]; then w=u nowho=0; fi
		;;
	who_go_swapped | a_not_o)
		x=$w w=
		while [ -n "$x" ]; do
			c=${x%"${x#?}"}
			x=${x#?}
			case $M:$c in
			who_go_swapped:g) c=o ;;
			who_go_swapped:o) c=g ;;
			a_not_o:a) c=ug ;;
			esac
			w=$w$c
		done
		;;
	esac
	tu=0 tg=0 to=0
	if [ "$nowho" = 1 ]; then
		tu=1 tg=1 to=1
	else
		x=$w
		while [ -n "$x" ]; do
			c=${x%"${x#?}"}
			x=${x#?}
			case $c in
			u) tu=1 ;;
			g) tg=1 ;;
			o) to=1 ;;
			a) tu=1 tg=1 to=1 ;;
			esac
		done
	fi
	set --
	while [ -n "$cl" ]; do
		op=${cl%"${cl#?}"}
		cl=${cl#?}
		case $op in
		[+=-]) ;;
		*)
			echo "chmod: invalid mode" >&2
			exit 1
			;;
		esac
		pl=
		while :; do
			case $cl in
			'' | [+=-]*) break ;;
			*)
				pl=$pl${cl%"${cl#?}"}
				cl=${cl#?}
				;;
			esac
		done
		set -- "$@" "$op$pl"
	done
	lim=$#
	case $M in
	actions_reverse)
		for a in "$@"; do set -- "$a" "$@"; done
		;;
	actions_first_only) lim=1 ;;
	actions_last_only)
		shift $(($# - 1))
		lim=1
		;;
	esac
	k=0
	for a in "$@"; do
		k=$((k + 1))
		[ "$k" -gt "$lim" ] && break
		if [ "$M" = action_who_lost ] && [ "$k" -gt 1 ]; then
			tu=1 tg=1 to=1 nowho=0
		fi
		pc_chmodref_act "$a"
	done
}

pc_chmodref_sym() {
	rest=$1
	[ "$M" = comma_last_only ] && rest=${rest##*,}
	more=1
	while [ "$more" = 1 ]; do
		case $rest in
		*,*)
			cl0=${rest%%,*}
			rest=${rest#*,}
			;;
		*)
			cl0=$rest rest= more=0
			;;
		esac
		pc_chmodref_clause "$cl0"
		[ "$M" = comma_first_only ] && break
	done
	# Choices the text leaves to the implementation, for regular files.
	if [ "$isreg" = 1 ]; then
		case $M in
		xclear_clears_s)
			if [ $((cur0 & 0111)) -ne 0 ] && [ $((new & 0111)) -eq 0 ]; then
				new=$((new & ~06000))
			fi
			;;
		s_ignored_noexec)
			if [ $((new & 0111)) -eq 0 ]; then
				new=$(((new & ~06000) | (cur0 & 06000)))
			fi
			;;
		esac
	fi
}

# Works the new mode of $1 out into "new".
pc_chmodref_calc() {
	pc_chmodref_stat "$1" || return 1
	cur0=$cur
	new=$cur
	case $mode in
	*[!0-7]* | '') pc_chmodref_sym "$mode" ;;
	*)
		if [ "$M" = octal_leading_zero_err ]; then
			case $mode in
			0?*)
				echo "chmod: invalid mode" >&2
				exit 1
				;;
			esac
		fi
		pc_chmodref_oct "$mode"
		;;
	esac
	return 0
}

# Changes one file ($2 is the depth below the operand).
pc_chmodref_do() {
	pc_chmodref_calc "$1" || {
		pc_chmodref_diag "$1"
		return 1
	}
	case $mode in
	*[!0-7]* | '') ;;
	*) [ "$M" = octal_ignored ] && return 0 ;;
	esac
	if [ "$M" = R_mode_from_top ]; then
		if [ "$2" -eq 0 ]; then topnew=$new; else new=$topnew; fi
	fi
	oct=$(printf '%04o' "$new")
	command chmod "$oct" "$1"
}

pc_chmodref_walk() {
	wp=$1 wdep=$2 wisd=0 wskip=0 wrc=0
	[ -d "$wp" ] && [ ! -L "$wp" ] && wisd=1
	case $M in
	R_dirs_skipped) [ "$wisd" = 1 ] && wskip=1 ;;
	R_files_skipped) [ "$wisd" = 0 ] && wskip=1 ;;
	R_file_operand_skipped) [ "$wisd" = 0 ] && [ "$wdep" -eq 0 ] && wskip=1 ;;
	esac
	if [ "$wskip" = 0 ] && [ "$post" = 0 ]; then
		pc_chmodref_do "$wp" "$wdep" || wrc=1
	fi
	if [ "$wisd" = 1 ] && [ -n "$R" ]; then
		wgo=1
		case $M in
		R_ignored) wgo=0 ;;
		R_depth1) [ "$wdep" -ge 1 ] && wgo=0 ;;
		esac
		if [ "$wgo" = 1 ]; then
			wtot=0
			for we in "$wp"/* "$wp"/.[!.]* "$wp"/..?*; do
				[ -e "$we" ] || [ -L "$we" ] || continue
				wtot=$((wtot + 1))
			done
			wn=0
			for we in "$wp"/* "$wp"/.[!.]* "$wp"/..?*; do
				[ -e "$we" ] || [ -L "$we" ] || continue
				case ${we##*/} in
				.*) [ "$M" = R_dotfiles_skipped ] && continue ;;
				esac
				wn=$((wn + 1))
				[ "$M" = R_first_entry ] && [ "$wn" -gt 1 ] && break
				[ "$M" = R_last_skipped ] && [ "$wn" -eq "$wtot" ] && break
				(pc_chmodref_walk "$we" $((wdep + 1))) || wrc=1
			done
		fi
	fi
	if [ "$wskip" = 0 ] && [ "$post" = 1 ]; then
		pc_chmodref_do "$wp" "$wdep" || wrc=1
	fi
	return $wrc
}

pc_chmodref() (
	M=$1
	shift
	R=
	while [ $# -gt 0 ]; do
		case $1 in
		-R)
			R=1
			shift
			;;
		--)
			shift
			break
			;;
		-[!R]*)
			if [ "$M" = strict_opts ]; then
				echo "chmod: invalid option -- ${1#-}" >&2
				exit 2
			fi
			break
			;;
		*) break ;;
		esac
	done
	if [ $# -lt 2 ]; then
		echo "chmod: missing operand" >&2
		exit 2
	fi
	mode=$1
	shift
	um=$((0$(umask)))
	post=0
	[ "$M" = R_dirs_last ] && post=1
	xfirst=
	case $M in
	operand_first_only) set -- "$1" ;;
	operand_last_only) eval 'set -- "${'$#'}"' ;;
	esac
	rc=0 k=0 skipnext=0
	for f in "$@"; do
		k=$((k + 1))
		if [ "$skipnext" = 1 ]; then
			skipnext=0
			continue
		fi
		case $f in
		-*) f=./$f ;;
		esac
		pc_chmodref_walk "$f" 0
		r=$?
		case $M in
		err_status_last) rc=$r ;;
		err_status_first) [ "$k" -eq 1 ] && rc=$r ;;
		*) [ "$r" -ne 0 ] && rc=1 ;;
		esac
		if [ "$r" -ne 0 ]; then
			[ "$M" = err_abort ] && break
			[ "$M" = err_skips_next ] && skipnext=1
		fi
	done
	case $M in
	err_status0) rc=0 ;;
	ok_status1) [ "$rc" -eq 0 ] && rc=1 ;;
	stdout_noise) echo done ;;
	esac
	exit "$rc"
)

# Shared by the csplit faults: a small reimplementation of csplit (POSIX
# locale, one planted fault chosen by name; any other name behaves correctly).
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# pc_csplitref FAULT ARGS...
pc_csplitref() (
	M=$1
	shift
	LC_ALL=C
	export LC_ALL
	NL='
'
	prefix=xx digits=2 keep= quiet= created= recs= cur=1 nfiles=0 nstart=0
	rxdone= opcount=0 inrep= lastkind= lastop= stride= lnval= src= tmpsrc=
	usage='csplit: usage: csplit [-ks] [-f prefix] [-n number] file arg...'
	OPTIND=1
	while getopts f:kn:s o; do
		case $o in
		f) prefix=$OPTARG ;;
		k) keep=1
			if [ "$M" = k_rejected ]; then
				echo "csplit: invalid option -- k" >&2
				exit 1
			fi ;;
		n) digits=$OPTARG ;;
		s) quiet=1 ;;
		*) echo "$usage" >&2; exit 1 ;;
		esac
	done
	shift $((OPTIND - 1))
	[ $# -ge 2 ] || { echo "$usage" >&2; exit 1; }
	file=$1
	shift
	case $M in
	s_ignored) quiet= ;;
	f_ignored) prefix=xx ;;
	n_ignored) digits=2 ;;
	n_plus1) digits=$((digits + 1)) ;;
	n_minus1) digits=$((digits - 1)) ;;
	f_basename) prefix=${prefix##*/} ;;
	default_digits_3) [ "$digits" = 2 ] && digits=3 ;;
	default_digits_1) [ "$digits" = 2 ] && digits=1 ;;
	file_start1) nstart=1 ;;
	k_no_sizes) [ -n "$keep" ] && quiet=1 ;;
	esac

	err() { # kind message
		ek=$1
		[ -z "$inrep" ] || [ "$ek" = missing ] || ek=repeat
		case $M in
		${ek}_no_diag) ;;
		${ek}_diag_stdout) echo "csplit: $2" ;;
		k_error_no_diag) [ -n "$keep" ] || echo "csplit: $2" >&2 ;;
		*) echo "csplit: $2" >&2 ;;
		esac
		if [ -z "$keep" ]; then
			case $M in
			${ek}_keeps) ;;
			${ek}_removes_some) [ -z "$created" ] || rm -f "${created##* }" ;;
			${ek}_removes_preexisting) rm -f $created "$prefix"* ;;
			*) [ -z "$created" ] || rm -f $created ;;
			esac
		fi
		[ -z "$tmpsrc" ] || rm -f "$tmpsrc"
		case $M in
		${ek}_status0) exit 0 ;;
		k_error_status0) [ -z "$keep" ] || exit 0 ;;
		esac
		exit 1
	}

	case $M in
	long_*)
		namemax=$(getconf NAME_MAX . 2>/dev/null)
		first=$(printf "%s%0${digits}d" "$prefix" 0)
		if [ "$namemax" -gt 0 ] 2>/dev/null && [ "${#first}" -gt "$namemax" ]; then
			case $M in
			long_truncate) prefix=$(printf '%s' "$prefix" | cut -c1-$((namemax - digits))) ;;
			long_creates)
				: >"$(printf '%s' "$first" | cut -c1-"$namemax")"
				err long "file name too long" ;;
			*) err long "file name too long" ;;
			esac
		fi ;;
	esac

	if [ "$file" = - ] && [ "$M" != dash_literal ] && [ "$M" != dash_file ]; then
		tmpsrc=${TMPDIR:-/tmp}/pcref.$$
		if [ "$M" = stdin_first_part ]; then head -n 5 >"$tmpsrc"; else cat >"$tmpsrc"; fi
		src=$tmpsrc
	else
		src=$file
	fi
	if [ ! -f "$src" ] || [ ! -r "$src" ]; then
		err missing "$file: cannot open"
	fi
	total=$(wc -l <"$src")
	total=$((total))
	[ "$M" != input_limit ] || [ "$total" -le 1000 ] || total=1000

	emit() { # first last: write lines first..last to the next file
		nm=$(printf "%s%0${digits}d" "$prefix" $((nfiles + nstart)))
		[ "$M" != n_hex ] || nm=$(printf "%s%0${digits}x" "$prefix" $((nfiles + nstart)))
		[ "$M" != n_spaces ] || nm=$(printf "%s%${digits}d" "$prefix" $((nfiles + nstart)))
		nfiles=$((nfiles + 1))
		if [ "$1" -le "$2" ]; then
			if [ "$M" = long_line_cut ]; then
				sed -n "$1,$2p" "$src" | sed 's/^\(.\{1000\}\).*$/\1/' >"$nm"
			else
				sed -n "$1,$2p" "$src" >"$nm"
			fi
		else
			: >"$nm"
		fi
		if [ "$M" = piece_nonl ]; then
			awk 'BEGIN{ORS=""} {if (NR>1) print "\n"; print}' "$nm" >"$nm.t" && mv "$nm.t" "$nm"
		fi
		created="$created $nm"
		eb=$(wc -c <"$nm")
		el=$(wc -l <"$nm")
		recs="$recs$((eb)) $((el)) $nm$NL"
	}

	ln_apply() { # N: the line number operand, already adjusted
		N=$1
		if [ "$N" -le "$cur" ]; then
			if [ "$M" = accept_backward ]; then
				emit 1 0
				return 0
			fi
			err back "$N: line number out of range"
		fi
		if [ "$N" -gt "$total" ]; then
			if [ "$M" = ln_end_plus1_ok ] && [ "$N" -eq $((total + 1)) ]; then
				emit "$cur" "$total"
				cur=$N
				return 0
			fi
			err past "$N: line number out of range"
		fi
		emit "$cur" $((N - 1))
		cur=$N
		[ "$M" != boundary_lost ] || cur=$((N + 1))
	}

	do_ln() { # operand text
		N=$1
		case $M in
		ln_plus1) N=$((N + 1)) ;;
		ln_minus1) N=$((N - 1)) ;;
		ln_first_digit) N=${N%${N#?}} ;;
		ln_relative) [ "$opcount" -eq 0 ] || N=$((cur + N)) ;;
		esac
		stride=$1
		lnval=$N
		ln_apply "$N"
	}

	do_rx() { # operand text; returns 1 when a repetition finds nothing and stops
		op=$1
		d=${op%${op#?}}
		rest=${op#?}
		offtext=${rest##*"$d"}
		re=${rest%"$d"*}
		case $M in
		plus_rejected) case $offtext in +*) err nomatch "$offtext: bad offset" ;; esac ;;
		esac
		case $M in
		slash_ends_expr) [ "$d" != / ] || re=${re%%\\/*} ;;
		percent_ends_expr) [ "$d" != % ] || re=${re%%\\%*} ;;
		*) re=$(printf '%s' "$re" | sed "s|\\\\$d|$d|g") ;;
		esac
		off=${offtext#+}
		[ -n "$off" ] || off=0
		if [ -n "$inrep" ] && [ "$M" = rep_offset_dropped ]; then off=0; fi
		case $M in
		off_plus_ignored) [ "$off" -le 0 ] || off=0 ;;
		off_minus_ignored) [ "$off" -ge 0 ] || off=0 ;;
		off_plus_extra) [ "$off" -le 0 ] || off=$((off + 1)) ;;
		off_plus_less) [ "$off" -le 0 ] || off=$((off - 1)) ;;
		off_minus_extra) [ "$off" -ge 0 ] || off=$((off - 1)) ;;
		off_minus_less) [ "$off" -ge 0 ] || off=$((off + 1)) ;;
		off_sign_swapped) off=$((0 - off)) ;;
		off_ignored) off=0 ;;
		off_first_digit)
			sg=
			case $off in -*) sg=- ;; esac
			dg=${off#-}
			off=$sg${dg%${dg#?}} ;;
		esac
		if [ "$rxdone" = 1 ] || [ "$cur" -gt 1 ]; then start=$((cur + 1)); else start=$cur; fi
		[ "$M" != match_from_cur ] || start=$cur
		gopt=
		case $M in
		match_ere) gopt=-E ;;
		match_literal) gopt=-F ;;
		match_icase) gopt=-i ;;
		esac
		m= seen=0
		for x in $(grep $gopt -n -e "$re" "$src" | cut -d: -f1); do
			[ "$x" -ge "$start" ] || continue
			[ "$x" -le "$total" ] || continue
			seen=$((seen + 1))
			if [ "$M" = match_second ] && [ "$seen" -lt 2 ]; then continue; fi
			m=$x
			[ "$M" = match_last ] || break
		done
		if [ -z "$m" ]; then
			if [ -n "$inrep" ] && [ "$M" = rep_nomatch_ok ]; then return 1; fi
			err nomatch "$re: match not found"
		fi
		t=$((m + off))
		[ "$M" != match_inclusive ] || t=$((t + 1))
		[ "$M" != match_exclusive_extra ] || t=$((t - 1))
		if [ "$t" -lt "$cur" ] || [ "$t" -gt "$total" ]; then
			err past "$op: line out of range"
		fi
		if [ "$d" = / ]; then
			emit "$cur" $((t - 1))
		else
			case $M in
			skip_writes_file) emit "$cur" $((t - 1)) ;;
			skip_gap) nfiles=$((nfiles + 1)) ;;
			skip_prints_size)
				eb=$(sed -n "$cur,$((t - 1))p" "$src" | wc -c)
				recs="$recs$((eb)) 0 skipped$NL" ;;
			esac
			case $M in
			skip_not_advances) t=$cur ;;
			skip_cur_plus1) t=$((t + 1)) ;;
			skip_cur_minus1) t=$((t - 1)) ;;
			skip_ignores_offset) t=$m ;;
			esac
		fi
		cur=$t
		rxdone=1
		return 0
	}

	do_rep() { # count
		cnt=$(($1))
		case $M in
		rep_extra) cnt=$((cnt + 1)) ;;
		rep_less) [ "$cnt" -le 0 ] || cnt=$((cnt - 1)) ;;
		rep_once) [ "$cnt" -le 0 ] || cnt=1 ;;
		rep_ignored) cnt=0 ;;
		rep_first_digit) cnt=${1%${1#?}} ;;
		rep_skip_ignored) [ "$lastkind" != skip ] || cnt=0 ;;
		esac
		inrep=1
		i=0
		while [ "$i" -lt "$cnt" ]; do
			i=$((i + 1))
			case $lastkind in
			ln)
				if [ "$M" = rep_ln_same ]; then ln_apply "$lnval"; else ln_apply $((cur + stride)); fi ;;
			*) do_rx "$lastop" || break ;;
			esac
		done
		inrep=
	}

	while [ $# -gt 0 ]; do
		op=$1
		shift
		case $op in
		'{'*'}')
			[ -n "$lastkind" ] || err nomatch "$op: no operand to repeat"
			n=${op#\{}
			n=${n%\}}
			do_rep "$n" ;;
		/*)
			do_rx "$op"
			lastkind=rx lastop=$op opcount=$((opcount + 1)) ;;
		%*)
			do_rx "$op"
			lastkind=skip lastop=$op opcount=$((opcount + 1)) ;;
		*[!0-9]* | '')
			err nomatch "$op: bad operand" ;;
		*)
			do_ln "$op"
			lastkind=ln opcount=$((opcount + 1)) ;;
		esac
	done
	if [ "$cur" -le "$total" ] && [ "$M" != last_dropped ]; then
		emit "$cur" "$total"
	fi
	[ "$M" != extra_empty_file ] || emit 1 0

	if [ "$M" = names_reversed ]; then
		j=$nfiles
		for nm in $created; do
			j=$((j - 1))
			mv "$nm" "pcref.mv.$j"
		done
		j=0
		while [ "$j" -lt "$nfiles" ]; do
			mv "pcref.mv.$j" "$(printf "%s%0${digits}d" "$prefix" $((j + nstart)))"
			j=$((j + 1))
		done
	fi
	if [ "$M" = k_no_files ] && [ -n "$keep" ]; then rm -f $created; fi
	[ -z "$tmpsrc" ] || rm -f "$tmpsrc"

	show=1
	[ -z "$quiet" ] || show=
	[ "$M" != size_none ] || show=
	case $M in
	s_to_stderr | s_partial) [ -z "$quiet" ] || show=1 ;;
	esac
	case $M in
	size_stderr) exec 1>&2 ;;
	s_to_stderr) [ -z "$quiet" ] || exec 1>&2 ;;
	esac
	if [ -n "$show" ]; then
		list=$recs
		if [ "$M" = size_reversed ]; then
			list=
			while IFS= read -r r; do
				[ -z "$r" ] || list="$r$NL$list"
			done <<EOF
$recs
EOF
		fi
		k=0
		nrecs=$(printf '%s' "$recs" | grep -c '')
		while IFS=' ' read -r b l nm; do
			[ -n "$b" ] || continue
			k=$((k + 1))
			if [ "$M" = size_last_missing ] && [ "$k" -eq "$nrecs" ]; then continue; fi
			if [ "$M" = s_partial ] && [ -n "$quiet" ] && [ "$k" -eq 1 ]; then continue; fi
			v=$b
			case $M in
			size_lines) v=$l ;;
			size_plus1) v=$((b + 1)) ;;
			esac
			case $M in
			size_nonl) printf '%s ' "$v" ;;
			size_named) printf '%s: %s\n' "$nm" "$v" ;;
			size_blank) printf '%s\n\n' "$v" ;;
			*) printf '%s\n' "$v" ;;
			esac
		done <<EOF
$list
EOF
	fi
	case $M in
	status1) exit 1 ;;
	k_status1) [ -z "$keep" ] || exit 1 ;;
	s_status1) [ -z "$quiet" ] || exit 1 ;;
	esac
	exit 0
)

# Shared by the xargs faults and variants: a small reimplementation of xargs
# for the POSIX locale (a character is a byte) with one planted fault or
# variant, chosen by name. Logical end-of-file is off unless -E is given.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# pc_xargsref NAME ARGS...: behaves correctly unless NAME applies.
pc_xargsref_awk=${PC_MUTANT_FILE%/*}/../mutants/xargsref.awk
pc_xargsref() (
	M=$1
	shift
	LC_ALL=C
	export LC_ALL
	zero=0 hasE=0 eof= repl= lnum= nmax=0 size=0 rflag=0 tflag=0 xflag=0
	[ "$M" = default_eof_underscore ] && { hasE=1 eof=_; }
	OPTIND=1
	while getopts 0E:I:L:n:s:prtx o; do
		case $o in
		0) zero=1 ;;
		E) hasE=1 eof=$OPTARG ;;
		I) repl=$OPTARG ;;
		L) lnum=$OPTARG ;;
		n) nmax=$OPTARG ;;
		s) size=$OPTARG ;;
		p) ;;
		r) rflag=1 ;;
		t) tflag=1 ;;
		x) xflag=1 ;;
		*) echo "xargs: invalid option" >&2; exit 2 ;;
		esac
	done
	shift $((OPTIND - 1))
	[ "$M" = i_fixed_braces ] && [ -n "$repl" ] && repl='{}'
	[ $# -eq 0 ] && set -- echo
	[ "$M" = default_printf ] && [ $# -eq 1 ] && [ "$1" = echo ] && set -- printf '%s\n'
	# a size above what is supported is cut down to the supported one
	if [ "$size" -gt 0 ]; then
		[ "$M" = s_big_error ] && [ "$size" -gt 100000000 ] && { echo "xargs: size too large" >&2; exit 1; }
		cap=100000
		[ "$M" = s_cap_small ] && cap=4096
		[ "$size" -gt "$cap" ] && size=$cap
	else
		size=32768
		[ "$M" = default_size_2048 ] && size=2048
	fi
	case $1 in
	*/*) : ;;
	*) [ "$M" = nopath_search ] && { echo "xargs: $1: not found" >&2; exit 127; } ;;
	esac
	nin=$#
	ulen=${#1}
	base=0
	k=0
	for a in "$@"; do
		k=$((k + 1))
		eval "in$k=\$a"
		base=$((base + ${#a} + 1))
	done
	d=pcxargsref.$$
	rm -rf "$d"
	mkdir "$d" || exit 1
	trap 'rm -rf "$d"' EXIT
	mode=def
	[ -n "$repl" ] && mode=I
	[ -n "$lnum" ] && mode=L
	[ "$mode" = I ] && xflag=1
	[ "$M" = i_nox ] && xflag=0
	od -An -v -tu1 | awk -v M="$M" -v dir="$d" -v zero="$zero" -v hasE="$hasE" \
		-v eof="$eof" -v mode="$mode" -v L="${lnum:-0}" -v n="$nmax" -v s="$size" \
		-v xflag="$xflag" -v rflag="$rflag" -v base="$base" -v nstr="$nin" \
		-v ulen="$ulen" -f "$pc_xargsref_awk" || exit 1

	# trace: the command line as it will be run
	trace() {
		[ "$M" = t_no_trace ] && return 0
		[ "$M" = t_once ] && [ "$nrun" -gt 1 ] && return 0
		tr_args=$#
		[ "$M" = t_no_utility ] && shift
		if [ "$M" = t_missing_initial ] && [ "$mode" != I ]; then
			tu=$1
			shift "$nin"
			set -- "$tu" "$@"
		fi
		sepc=' '
		[ "$M" = trace_tab ] && sepc='	'
		line= first=1
		for a in "$@"; do
			if [ $first = 1 ]; then line=$a first=0; else line=$line$sepc$a; fi
		done
		if [ "$M" = t_stdout ]; then printf '%s\n' "$line"; else printf '%s\n' "$line" >&2; fi
	}
	nrun=0 stopnow=0 anyfail=0 stat_first=0 stat_max=0 stat_last=0 stat_sum=0 lastrc=0 more=-1
	exitcode=
	for f in "$d"/b.*; do
		[ -e "$f" ] || break
		nrun=$((nrun + 1))
		if [ "$mode" = I ]; then
			eval "set -- $(cat "$f")"
			line=$*
			[ "$M" = i_batches ] || line=$1
			i=1 replaced=0
			set --
			while [ $i -le $nin ]; do
				eval "a=\$in$i"
				if [ $i -gt 1 ]; then
					res= rest=$a one=0
					while :; do
						case $rest in *"$repl"*) : ;; *) break ;; esac
						[ "$M" = i_args3 ] && [ $i -gt 4 ] && break
						[ "$M" = i_one_arg ] && [ $replaced = 1 ] && [ $one = 0 ] && break
						[ "$M" = i_first_only ] && [ $one = 1 ] && break
						res=$res${rest%%"$repl"*}$line
						rest=${rest#*"$repl"}
						one=1 replaced=1
					done
					a=$res$rest
				fi
				set -- "$@" "$a"
				i=$((i + 1))
			done
			len=0
			for a in "$@"; do len=$((len + ${#a} + 1)); done
			if [ "$xflag" = 1 ] && [ "$len" -ge "$size" ]; then
				echo "xargs: argument list too long" >&2
				exitcode=1
				break
			fi
		else
			eval "set -- $(cat "$f")"
			nb=$#
			j=0
			for a in "$@"; do j=$((j + 1)); eval "bb$j=\$a"; done
			set --
			if [ "$M" = init_after ]; then
				set -- "$in1"
				j=1
				while [ $j -le "$nb" ]; do eval "set -- \"\$@\" \"\$bb$j\""; j=$((j + 1)); done
				j=2
				while [ $j -le "$nin" ]; do eval "set -- \"\$@\" \"\$in$j\""; j=$((j + 1)); done
			else
				j=1
				while [ $j -le "$nin" ]; do
					[ "$M" = init_dropped ] && [ $j -gt 1 ] && break
					eval "set -- \"\$@\" \"\$in$j\""
					j=$((j + 1))
				done
				j=1
				while [ $j -le "$nb" ]; do eval "set -- \"\$@\" \"\$bb$j\""; j=$((j + 1)); done
			fi
		fi
		[ "$tflag" = 1 ] && [ "$M" != t_after ] && trace "$@"
		if [ "$M" = t_dryrun ] && [ "$tflag" = 1 ]; then continue; fi
		{ "$@" </dev/null; } 2>"$d/err"
		rc=$?
		if [ "$M" = nf_no_diag ] && { [ $rc -eq 126 ] || [ $rc -eq 127 ]; }; then :; else cat "$d/err" >&2; fi
		[ "$tflag" = 1 ] && [ "$M" = t_after ] && trace "$@"
		if [ $rc -eq 127 ] || [ $rc -eq 126 ]; then
			[ "$M" = nf_no_diag ] || echo "xargs: $1: cannot run the utility" >&2
			case $M:$rc in
			nf_status1:127) exitcode=1 ;;
			nf_status126:127) exitcode=126 ;;
			nf_status0:127) exitcode=0 ;;
			swapped:127) exitcode=126 ;;
			swapped:126) exitcode=127 ;;
			noexec_127:126) exitcode=127 ;;
			noexec_1:126) exitcode=1 ;;
			noexec_0:126) exitcode=0 ;;
			*) exitcode=$rc ;;
			esac
			break
		fi
		if [ $rc -eq 255 ] && [ "$M" != s255_continues ]; then
			case $M in
			s255_no_diag) ;;
			s255_diag_stdout) echo "xargs: the utility exited with status 255" ;;
			*) echo "xargs: the utility exited with status 255" >&2 ;;
			esac
			case $M in
			s255_one_more) [ $more -lt 0 ] && { more=1; continue; } ;;
			esac
			case $M in
			s255_status0) exitcode=0 ;;
			s255_status255) exitcode=255 ;;
			*) exitcode=1 ;;
			esac
			break
		fi
		if [ $more -ge 0 ]; then exitcode=1; break; fi
		lastrc=$rc
		if [ $rc -ne 0 ]; then
			[ $rc -gt 125 ] && rc=1
			[ $anyfail = 0 ] && stat_first=$rc
			anyfail=1
			[ $rc -gt $stat_max ] && stat_max=$rc
			stat_last=$rc
			stat_sum=$((stat_sum + rc))
			[ "$M" = fail_stops ] && { exitcode=1; break; }
		fi
	done
	if [ -z "$exitcode" ] && [ -e "$d/fail" ]; then
		[ "$M" = x_no_diag ] || cat "$d/fail" >&2
		case $M in
		x_status0) exitcode=0 ;;
		x_status126) exitcode=126 ;;
		*) exitcode=1 ;;
		esac
	fi
	if [ -z "$exitcode" ]; then
		exitcode=0
		if [ $anyfail = 1 ]; then
			case $M in
			fail_status0) exitcode=0 ;;
			fail_status126) exitcode=126 ;;
			fail_last_only) if [ "$lastrc" -ne 0 ]; then exitcode=1; fi ;;
			status_first) exitcode=$stat_first ;;
			status_max) exitcode=$stat_max ;;
			status_last) exitcode=$stat_last ;;
			status_sum) exitcode=$stat_sum ;;
			*) exitcode=1 ;;
			esac
		fi
		[ "$M" = status1_ok ] && [ $exitcode = 0 ] && exitcode=1
		[ "$M" = x_always_error ] && [ "$xflag" = 1 ] && [ "$mode" != I ] && { echo "xargs: argument list too long" >&2; exitcode=1; }
		[ "$M" = noise_stdout ] && echo "xargs: done"
	fi
	exit "$exitcode"
)

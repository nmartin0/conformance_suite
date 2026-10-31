# Shared by the sort faults: a small reimplementation of sort for the POSIX
# locale with one planted fault, chosen by name (see sortref.awk).
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# pc_sortref FAULT ARGS...: behaves as sort does, except for the fault.
pc_sortref_awk=${PC_SR_DIR:-${PC_MUTANT_FILE%/*}}/sortref.awk
pc_sortref() (
	M=$1
	shift
	b=0 c=0 d=0 f=0 i=0 m=0 n=0 r=0 u=0 o= t= keys=
	OPTIND=1
	while getopts bcCdfimno:rt:uk: opt; do
		case $opt in
		b) b=1 ;;
		c) c=1 ;;
		C) c=2 ;;
		d) d=1 ;;
		f) f=1 ;;
		i) i=1 ;;
		m) m=1 ;;
		n) n=1 ;;
		r) r=1 ;;
		u) u=1 ;;
		o) o=$OPTARG ;;
		t) t=$OPTARG ;;
		k) keys=${keys:+$keys:}$OPTARG ;;
		*) exit 2 ;;
		esac
	done
	shift $((OPTIND - 1))
	case $M in
	n_ignored) n=0 ;;
	f_ignored) f=0 ;;
	d_ignored) d=0 ;;
	i_ignored) i=0 ;;
	r_ignored) r=0 ;;
	t_ignored) t= ;;
	m_ignores_r) [ "$m" = 1 ] && r=0 ;;
	c_ignores_opts) [ "$c" != 0 ] && { b=0 d=0 f=0 i=0 n=0 r=0 keys=; } ;;
	esac
	tmpin=pcsr-in.$$
	tmpout=pcsr-out.$$
	trap 'rm -f "$tmpin" "$tmpin".stdin "$tmpout"' EXIT
	: >"$tmpin"
	if [ $# -eq 0 ]; then
		[ "$M" = noargs_empty ] && exit 0
		set -- -
	elif [ "$M" = stdin_also ]; then
		set -- "$@" -
	fi
	if [ "$M" = only_last_file ]; then
		for fl; do last=$fl; done
		set -- "$last"
	fi
	[ "$M" = o_early ] && [ -n "$o" ] && : >"$o"
	late=0
	for fl; do
		src=$fl
		if [ "$fl" = - ] && [ "$M" != dash_literal ]; then
			[ "$M" = dash_skipped ] && continue
			cat >"$tmpin.stdin"
			src=$tmpin.stdin
		fi
		if [ ! -r "$src" ] || [ -d "$src" ]; then
			echo "sort: $fl: cannot open" >&2
			case $M in
			unreadable_status0) late=1; continue ;;
			unreadable_partial) late=2; continue ;;
			unreadable_status1) exit 1 ;;
			*) exit 2 ;;
			esac
		fi
		inc=0
		[ -n "$(tail -c 1 "$src")" ] && inc=1
		if [ "$inc" = 1 ] && [ "$M" = incomplete_dropped ]; then
			sed '$d' "$src" >>"$tmpin"
		else
			cat "$src" >>"$tmpin"
			[ "$inc" = 1 ] && [ "$M" != glue_files ] && echo >>"$tmpin"
		fi
		[ "$M" = only_first_file ] && break
		[ "$M" = m_first_only ] && [ "$m" = 1 ] && break
	done
	awk -v M="$M" -v G_b=$b -v G_d=$d -v G_f=$f -v G_i=$i -v G_n=$n -v G_r=$r \
		-v U=$u -v C=$c -v MG=$m -v T="$t" -v KEYS="$keys" \
		-f "$pc_sortref_awk" <"$tmpin" >"$tmpout"
	arc=$?
	if [ "$c" != 0 ]; then
		cat "$tmpout"
	elif [ -n "$o" ] && [ "$M" != o_ignored ]; then
		if [ "$M" = o_append ]; then cat "$tmpout" >>"$o"; else cat "$tmpout" >"$o"; fi || exit 2
		[ "$M" = o_both ] && cat "$tmpout"
		[ "$M" = o_status1 ] && exit 1
	else
		cat "$tmpout"
	fi
	[ "$arc" -eq 0 ] && [ "$late" = 2 ] && exit 1
	exit "$arc"
)

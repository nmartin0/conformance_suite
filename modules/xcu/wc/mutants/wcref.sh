# Shared by the wc faults and variants: a small reimplementation of wc
# that takes its counts from the real utility and plants one fault (or one
# permitted choice), chosen by name.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# pc_wcref NAME ARGS...: behaves as wc should, except for what NAME says.
pc_wcref() (
	M=$1
	shift
	fl= fw= fc= fm= ord= nopt=0 lastopt=
	OPTIND=1
	while getopts clmw o; do
		case $o in
		l) fl=1 ;;
		w) fw=1 ;;
		c) fc=1 fm= ;;
		m) fm=1 fc= ;;
		*) echo "wc: invalid option" >&2; exit 1 ;;
		esac
		ord="$ord$o"
		nopt=$((nopt + 1))
		lastopt=$o
	done
	shift $((OPTIND - 1))
	case $M in
	opt_last_wins) fl= fw= fc= fm=
		case $lastopt in l) fl=1 ;; w) fw=1 ;; c) fc=1 ;; m) fm=1 ;; esac ;;
	l_ignored) fl= ;;
	w_ignored) fw= ;;
	c_ignored) fc= ;;
	m_ignored) fm= ;;
	l_all | w_all | c_all)
		case $M in l_all) [ -n "$fl" ] ;; w_all) [ -n "$fw" ] ;; c_all) [ -n "$fc" ] ;; esac &&
			{ fl=1 fw=1 fc=1 fm=; } ;;
	opt_extra_l) [ "$nopt" -gt 0 ] && fl=1 ;;
	opt_extra_w) [ "$nopt" -gt 0 ] && fw=1 ;;
	opt_extra_c) [ "$nopt" -gt 0 ] && [ -z "$fm" ] && fc=1 ;;
	drop_l_combo) [ "$nopt" -gt 1 ] && fl= ;;
	esac
	if [ -z "$fl$fw$fc$fm" ]; then
		fl=1 fw=1 fc=1
		ord=lwc
		case $M in
		default_no_words) fw= ;;
		default_no_lines) fl= ;;
		default_no_bytes) fc= ;;
		esac
	fi
	[ -n "$fl" ] && [ -z "$ord" ] && ord=l
	t=pc-wcref.$$
	trap 'rm -f "$t" "$t.n"' EXIT
	cnt() {
		set -- $(command wc -"$1" <"$2")
		echo "$1"
	}
	words() {
		case $M in
		w_nl_joins) tr '\n' x <"$1" | tr '\t\r\f\v' '    ' | awk '{n+=NF}END{print n+0}' ;;
		w_tab_joins) tr '\t' x <"$1" | tr '\r\f\v' '   ' | awk '{n+=NF}END{print n+0}' ;;
		w_cr_joins) tr '\f\v' '  ' <"$1" | awk '{n+=NF}END{print n+0}' ;;
		w_ff_joins) tr '\r\v' '  ' <"$1" | awk '{n+=NF}END{print n+0}' ;;
		w_vt_joins) tr '\r\f' '  ' <"$1" | awk '{n+=NF}END{print n+0}' ;;
		w_lines) tr '\r\f\v' '   ' <"$1" | awk 'NF>0{n++}END{print n+0}' ;;
		w_empty_fields) tr '\t\r\f\v' '    ' <"$1" | awk -F'[ ]' '{n+=NF}END{print n+0}' ;;
		w_punct_splits) tr -c 'A-Za-z0-9\n' ' ' <"$1" | awk '{n+=NF}END{print n+0}' ;;
		*) tr '\r\f\v' '   ' <"$1" | awk '{n+=NF}END{print n+0}' ;;
		esac
	}
	lastbyte_not_nl() {
		[ -s "$1" ] || return 1
		set -- $(tail -c 1 "$1" | od -An -tx1)
		[ "$1" != 0a ]
	}
	# counts of one file into the variables l w c m
	count() {
		l=$(cnt l "$1") c=$(cnt c "$1") m=$(cnt m "$1")
		w=$(words "$1")
		case $M in
		l_off) l=$((l + 1)) ;;
		w_off) w=$((w + 1)) ;;
		c_off) c=$((c + 1)) ;;
		l_partial) lastbyte_not_nl "$1" && l=$((l + 1)) ;;
		l_skip_empty) l=$((l - $(grep -c '^$' "$1"))) ;;
		l_empty1) [ -s "$1" ] || l=1 ;;
		l_cr) l=$((l + $(tr -cd '\r' <"$1" | command wc -c))) ;;
		w_last_dropped) lastbyte_not_nl "$1" && [ "$w" -gt 0 ] && w=$((w - 1)) ;;
		c_nul_stop) c=$(od -An -v -tx1 <"$1" | tr -s ' \n' '\n\n' | sed -n '/^00$/q;/./p' | command wc -l)
			c=${c##* } ;;
		c_high_dropped) c=$(LC_ALL=POSIX tr -d '\200-\377' <"$1" | command wc -c) ;;
		m_as_lines) m=$l ;;
		m_as_words) m=$w ;;
		m_bytes) m=$c ;;
		wrap16) l=$((l % 65536)) w=$((w % 65536)) c=$((c % 65536)) m=$((m % 65536)) ;;
		cap6) for v in l w c m; do eval "[ \$$v -gt 999999 ] && $v=999999"; done ;;
		esac
		set -- $l $w $c $m
	}
	line() { # name l w c m
		nm=$1 lv=$2 wv=$3 cv=$4 mv=$5
		out=
		order=lwc
		[ "$M" = swap_lw ] && order=wlc
		case $M in order_args) [ "$nopt" -gt 0 ] && order=$(printf '%s' "$ord" | tr m c) ;; esac
		o=$order
		while [ -n "$o" ]; do
			ch=${o%"${o#?}"}
			o=${o#?}
			case $ch in
			l) [ -n "$fl" ] && out="$out $lv" ;;
			w) [ -n "$fw" ] && out="$out $wv" ;;
			c)
				[ -n "$fc" ] && out="$out $cv"
				[ -n "$fm" ] && [ "$M" = m_adds_c ] && out="$out $cv"
				[ -n "$fm" ] && out="$out $mv" ;;
			esac
		done
		set -- $out
		out=
		for v; do
			[ "$M" = padded ] && v=$(printf '%7s' "$v")
			case $M in comma_sep) out="$out${out:+,}$v" ;; *) out="$out${out:+ }$v" ;; esac
		done
		if [ -n "$nm" ]; then
			case $M in
			name_first) out="$nm $out" ;;
			name_altered) out="$out ./$nm" ;;
			name_missing) ;;
			*) out="$out $nm" ;;
			esac
		fi
		[ "$M" = stdin_trailing_blank ] && [ -z "$nm" ] && out="$out "
		if [ "$M" = no_newline ]; then printf '%s' "$out"
		elif [ "$M" = out_stderr ]; then printf '%s\n' "$out" >&2
		else printf '%s\n' "$out"; fi
	}
	status=0
	if [ $# -eq 0 ]; then
		case $M in
		stdin_head) head -c 512 >"$t" ;;
		*) cat >"$t" ;;
		esac
		count "$t"
		case $M in
		stdin_zero) l=0 w=0 c=0 m=0 ;;
		esac
		nm=
		[ "$M" = stdin_name_dash ] && nm=-
		line "$nm" "$l" "$w" "$c" "$m"
		[ "$M" = status1 ] && exit 1
		exit 0
	fi
	if [ "$M" = reverse_files ]; then
		i=0
		for f; do
			i=$((i + 1))
			eval "v$i=\$f"
		done
		set --
		while [ "$i" -ge 1 ]; do
			eval 'set -- "$@" "$v'$i'"'
			i=$((i - 1))
		done
	fi
	nfiles=$#
	tl=0 tw=0 tc=0 tm=0 first= done_n=0 buf=$t.n
	: >"$buf"
	idx=0
	for f; do
		idx=$((idx + 1))
		if [ ! -r "$f" ] || [ -d "$f" ]; then
			case $M in
			miss_no_diag) ;;
			miss_diag_stdout) echo "wc: $f: cannot be read" ;;
			*) echo "wc: $f: cannot be read" >&2 ;;
			esac
			case $M in
			miss_status0) ;;
			miss_status_last) [ "$idx" -eq "$nfiles" ] && status=1 ;;
			miss_status_first) [ "$idx" -eq 1 ] && status=1 ;;
			miss_status2 | status2) status=2 ;;
			status255) status=255 ;;
			*) status=1 ;;
			esac
			[ "$M" = miss_abort ] && exit $status
			continue
		fi
		count "$f"
		if [ "$idx" -eq 1 ] || [ -z "$first" ]; then
			first=$f fl1=$l fw1=$w fc1=$c fm1=$m
		fi
		lastl=$l lastw=$w lastc=$c lastm=$m
		if [ "$M" != total_two_only ] || [ "$idx" -le 2 ]; then
			tl=$((tl + l)) tw=$((tw + w)) tc=$((tc + c)) tm=$((tm + m))
		fi
		line "$f" "$l" "$w" "$c" "$m" >>"$buf"
		done_n=$((done_n + 1))
	done
	# a total line, for more than one operand
	tot=
	if [ "$nfiles" -gt 1 ] || [ "$M" = total_single ]; then
		case $M in
		total_missing) ;;
		total_none_on_error) [ "$status" -eq 0 ] && tot=total ;;
		*) tot=total ;;
		esac
	fi
	if [ "$M" = miss_discard ] && [ "$status" -ne 0 ]; then
		exit $status
	fi
	if [ -n "$tot" ]; then
		nm=total
		case $M in
		total_name) nm=sum ;;
		total_last_only) tl=$lastl tw=$lastw tc=$lastc tm=$lastm ;;
		total_first_only) tl=$fl1 tw=$fw1 tc=$fc1 tm=$fm1 ;;
		total_all_cols) fl=1 fw=1 fc=1 fm= ;;
		esac
		line "$nm" "$tl" "$tw" "$tc" "$tm" >"$t.tot"
		if [ "$M" = total_first ]; then cat "$t.tot" "$buf"; else cat "$buf" "$t.tot"; fi
		rm -f "$t.tot"
	else
		cat "$buf"
	fi
	[ "$M" = status1 ] && [ "$status" -eq 0 ] && status=1
	exit $status
)

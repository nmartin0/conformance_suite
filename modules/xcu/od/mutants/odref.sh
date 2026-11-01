# Shared by the od faults and variants: a wrapper that runs the real utility
# and plants one fault (or one permitted choice), chosen by name.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# pc_odref NAME ARGS...: behaves as od should, except for what NAME says.
# The options are read (-A -j -N -t -v and the letters -b -c -d -o -s -x),
# turned into a list of output types, and handed to the real utility; the
# input, the arguments, the output or the status is altered by NAME.
pc_odref() (
	M=$1
	shift
	NL='
'
	A= jv= nv= vf= tl= ng=0 anyT=
	addstr() { # string group
		s=$1
		while [ -n "$s" ]; do
			c=${s%"${s#?}"}
			s=${s#?}
			tok=$c
			case $c in
			[douxf])
				case $c:$s in
				?:[0-9]*)
					dg=${s%%[!0-9]*}
					tok=$tok$dg
					s=${s#"$dg"} ;;
				[douxX]:[CSIL]*)
					tok=$tok${s%"${s#?}"}
					s=${s#?} ;;
				f:[FDL]*)
					tok=$tok${s%"${s#?}"}
					s=${s#?} ;;
				esac ;;
			esac
			tl="$tl$2 T $c $tok$NL"
		done
	}
	letter() { # letter group
		case $1 in
		b) t=o1 ;;
		c) t=c ;;
		d) t=u2 ;;
		o) t=o2 ;;
		s) t=d2 ;;
		x) t=x2 ;;
		esac
		case $M:$1 in
		lb_x1:b) t=x1 ;;
		lc_a:c) t=a ;;
		ld_d2:d) t=d2 ;;
		ls_u2:s) t=u2 ;;
		lo_o1:o) t=o1 ;;
		lx_x1:x) t=x1 ;;
		esac
		[ "$M" = l_ignored ] || tl="$tl$2 L $1 $t$NL"
	}
	while [ $# -gt 0 ]; do
		case $1 in
		-A) A=$2; shift 2 ;;
		-A?*) A=${1#-A}; shift ;;
		-j) jv=$2; shift 2 ;;
		-j?*) jv=${1#-j}; shift ;;
		-N) nv=$2; shift 2 ;;
		-N?*) nv=${1#-N}; shift ;;
		-t) ng=$((ng + 1)); anyT=1; addstr "$2" $ng; shift 2 ;;
		-t?*) ng=$((ng + 1)); anyT=1; addstr "${1#-t}" $ng; shift ;;
		-v) vf=1; shift ;;
		--) shift; break ;;
		-[bcdosxv]*)
			cl=${1#-}
			shift
			while [ -n "$cl" ]; do
				ch=${cl%"${cl#?}"}
				cl=${cl#?}
				if [ "$ch" = v ]; then vf=1; else ng=$((ng + 1)); letter "$ch" $ng; fi
			done ;;
		-?*) echo "od: invalid option" >&2; exit 1 ;;
		*) break ;;
		esac
	done

	# --- the XSI offset operand: the real utility may lack it, so it is
	# done here for the faults of that feature (and its reference).
	case $M in
	xsi_good | xo_*)
		jraw=
		ok=
		if [ $# -ge 1 ] && [ $# -le 2 ]; then
			for last; do :; done
			case $last in
			+*) ok=1 ;;
			[0-9]*) [ $# -eq 2 ] && ok=1 ;;
			esac
			[ -n "$anyT" ] && [ -n "$(printf '%s' "$tl" | awk '$2=="T"')" ] && ok=
			[ -n "$A$jv$nv$vf" ] && ok=
		fi
		case $M in
		xo_always) [ $# -ge 1 ] && [ $# -le 2 ] && { for last; do :; done; case $last in +*) ok=1 ;; [0-9]*) [ $# -eq 2 ] && ok=1 ;; esac; } ;;
		xo_three) [ $# -eq 3 ] && { for last; do :; done; case $last in +*) ok=1 ;; [0-9]*) ok=1 ;; esac; } ;;
		xo_num_single) [ $# -eq 1 ] && { case $1 in [0-9]*) ok=1 ;; esac; } ;;
		xo_plus_file) [ $# -eq 1 ] && case $1 in +*) ok= ;; esac ;;
		xo_num_file) [ $# -eq 2 ] && case $2 in [0-9]*) ok= ;; esac ;;
		xo_plus_only) case $last in +*) ;; *) ok= ;; esac ;;
		esac
		if [ -n "$ok" ]; then
			for last; do :; done
			o=${last#+}
			mult=1
			case $o in
			*b) o=${o%b}; mult=512 ;;
			esac
			dot=
			case $o in
			*.) o=${o%.}; dot=1 ;;
			esac
			case $M in
			xo_b_ignored) mult=1 ;;
			xo_b_1024) [ $mult = 512 ] && mult=1024 ;;
			xo_dot_octal) dot= ;;
			xo_octal_decimal) dot=1 ;;
			xo_dot_hex) ;;
			esac
			if [ -n "$dot" ]; then
				while :; do case $o in 0?*) o=${o#0} ;; *) break ;; esac; done
				v=$((o))
			else
				v=$((0$o))
			fi
			v=$((v * mult))
			[ "$M" = xo_ignored ] && v=0
			jv=$v
			n=$#
			while [ $n -gt 1 ]; do
				set -- "$@" "$1"
				shift
				n=$((n - 1))
			done
			shift
			n=$#
			# keep the leading operands in order
			:
		fi ;;
	esac

	# --- the arguments for the real utility
	case $M in
	Ad_oct) [ "$A" = d ] && A=o ;;
	Ao_dec) [ "$A" = o ] && A=d ;;
	Ax_dec) [ "$A" = x ] && A=d ;;
	Ax_oct) [ "$A" = x ] && A=o ;;
	An_dec) [ "$A" = n ] && A=d ;;
	A_default_d) [ -n "$A" ] || A=d ;;
	A_default_x) [ -n "$A" ] || A=x ;;
	esac

	dec() { # value with an optional 0x/0X/0 prefix, no suffix
		echo $(($1))
	}
	nostrip() {
		case $1 in
		0[xX]*) echo "$1" ;;
		*)
			z=$1
			while :; do case $z in 0?*) z=${z#0} ;; *) break ;; esac; done
			echo "$z" ;;
		esac
	}
	if [ -n "$jv" ]; then
		case $M in
		j_ignored) jv= ;;
		j_plus1) jv=$(($(dec "$jv") + 1)) ;;
		j_minus1) jv=$(($(dec "$jv") - 1)); [ "$jv" -lt 0 ] && jv=0 ;;
		j_oct_dec) jv=$(nostrip "$jv") ;;
		j_hex_0) case $jv in 0[xX]*) jv=0 ;; esac ;;
		j_suffix_ign) case $jv in 0[xX]*) ;; *[bkm]) jv=${jv%?} ;; esac ;;
		j_k_1000) case $jv in 0[xX]*) ;; *k) jv=$(($(dec "${jv%k}") * 1000)) ;; esac ;;
		j_b_1024) case $jv in 0[xX]*) ;; *b) jv=$(($(dec "${jv%b}") * 1024)) ;; esac ;;
		j_m_1000000) case $jv in 0[xX]*) ;; *m) jv=$(($(dec "${jv%m}") * 1000000)) ;; esac ;;
		j_hexb_mult) case $jv in 0[xX]*b) jv=$(($(dec "${jv%b}") * 512)) ;; esac ;;
		j_trunc32) case $jv in *[!0-9]*) ;; *) jv=$(($jv % 4294967296)) ;; esac ;;
		esac
	fi
	if [ -n "$nv" ]; then
		case $M in
		N_ignored) nv= ;;
		N_plus1) nv=$(($(dec "$nv") + 1)) ;;
		N_minus1) nv=$(($(dec "$nv") - 1)); [ "$nv" -lt 0 ] && nv=0 ;;
		N_oct_dec) nv=$(nostrip "$nv") ;;
		N_hex_0) case $nv in 0[xX]*) nv=0 ;; esac ;;
		N_before_j) [ -n "$jv" ] && nv=$(($(dec "$nv") - $(dec "$jv"))) ;;
		esac
	fi
	case $M in
	v_ignored) vf= ;;
	star_never) vf=1 ;;
	esac

	# --- the order and form of the output types
	tlist=$(printf '%s' "$tl" | awk -v M="$M" '
		{ g[NR] = $1; k[NR] = $2; key[NR] = $3; t[NR] = $4 }
		END {
			n = NR
			anyL = 0
			for (i = 1; i <= n; i++) if (k[i] == "L") anyL = 1
			if (M == "t_reversed" || (M == "l_reversed" && anyL)) {
				for (i = n; i >= 1; i--) print t[i]
			} else if (M == "t_opts_reversed") {
				for (gg = g[n]; gg >= 1; gg--)
					for (i = 1; i <= n; i++) if (g[i] == gg) print t[i]
			} else if (M == "t_string_reversed") {
				for (gg = 1; gg <= g[n]; gg++)
					for (i = n; i >= 1; i--) if (g[i] == gg) print t[i]
			} else if (M == "t_first_only") {
				if (n) print t[1]
			} else if (M == "t_last_only") {
				if (n) print t[n]
			} else if (M == "t_string_first_only") {
				for (i = 1; i <= n; i++) if (i == 1 || g[i] != g[i-1]) print t[i]
			} else if (M == "l_last_only" && anyL) {
				print t[n]
			} else if (M == "l_sorted" && anyL) {
				for (i = 1; i <= n; i++) o[i] = i
				for (i = 1; i <= n; i++) for (j = i + 1; j <= n; j++)
					if (key[o[j]] < key[o[i]]) { x = o[i]; o[i] = o[j]; o[j] = x }
				for (i = 1; i <= n; i++) print t[o[i]]
			} else {
				for (i = 1; i <= n; i++) print t[i]
			}
		}')
	fixtok() {
		tok=$1
		case $M in
		sz8_to_4) case $tok in [douxX]8) tok=${tok%8}4 ;; esac ;;
		sz4_to_2) case $tok in [douxX]4) tok=${tok%4}2 ;; esac ;;
		sz2_to_1) case $tok in [douxX]2) tok=${tok%2}1 ;; esac ;;
		sz_all_1) case $tok in [douxX][248]) tok=${tok%?}1 ;; esac ;;
		o_as_x) case $tok in o*) tok=x${tok#o} ;; esac ;;
		x_as_o) case $tok in x*) tok=o${tok#x} ;; esac ;;
		x_as_u) case $tok in x*) tok=u${tok#x} ;; esac ;;
		o_as_u) case $tok in o*) tok=u${tok#o} ;; esac ;;
		u_as_d) case $tok in u*) tok=d${tok#u} ;; esac ;;
		d_as_u) case $tok in d*) tok=u${tok#d} ;; esac ;;
		esac
	}
	RT=
	ntok=0
	firsttok=
	szt=1
	allint=1
	for tk in $tlist; do
		fixtok "$tk"
		RT="$RT -t $tok"
		ntok=$((ntok + 1))
		[ -n "$firsttok" ] || firsttok=$tok
		case $tk in
		[douxX][0-9]*) [ "$szt" = 1 ] && szt=${tk#?} ;;
		esac
		case $tk in [douxX]*) ;; *) allint= ;; esac
	done
	if [ $ntok -eq 0 ]; then
		case $M in
		dflt_x2) RT="-t x2" ;;
		dflt_o1) RT="-t o1" ;;
		dflt_o4) RT="-t o4" ;;
		dflt_o8) RT="-t o8" ;;
		dflt_u2) RT="-t u2" ;;
		dflt_c) RT="-t c" ;;
		dflt_none) RT="-N 0" ;;
		esac
	fi
	# the byte-oriented formatter faults replace the whole listing
	fmtkind=
	case $M in
	a_* | lf_name) [ "$tlist" = a ] && fmtkind=a ;;
	c_*) [ "$tlist" = c ] && fmtkind=c ;;
	esac
	[ "$A" = n ] || fmtkind=
	RA=
	JN=
	[ -n "$jv" ] && JN="$JN -j $jv"
	[ -n "$nv" ] && JN="$JN -N $nv"
	if [ -n "$fmtkind" ]; then
		RA="-A n -v -t o1$JN"
	else
		[ -n "$A" ] && RA="$RA -A $A"
		RA="$RA$JN"
		case $M in
		dedup_* | star_[cfb]* | block* | off_*layout*) ;;
		esac
		RA="$RA$RT"
		[ -n "$vf" ] && RA="$RA -v"
	fi
	dedup=
	case $M in
	star_first_replaced | star_cmp_first | star_first_token | star_blank | dedup_std)
		if [ -z "$vf" ]; then dedup=$M; RA="$RA -v"; fi ;;
	block8 | block32 | block1x)
		:
		;;
	esac

	# --- the input
	t=pc-odref.$$
	e=pc-odref.$$.e
	trap 'rm -f "$t" "$t.1" "$t.2" "$e"' EXIT
	: >"$e"
	total() { # bytes in all the operands
		cat -- "$@" 2>/dev/null | wc -c | tr -d ' \t'
	}
	havefiles=$#
	case $M in
	first_only) [ $# -gt 1 ] && set -- "$1" ;;
	last_only) [ $# -gt 1 ] && { for lastf; do :; done; set -- "$lastf"; } ;;
	stdin_ignored) [ $# -eq 0 ] && exec </dev/null ;;
	files_reversed)
		if [ $# -gt 1 ]; then
			rv=$#
			while [ $rv -gt 0 ]; do
				eval "set -- \"\$@\" \"\${$rv}\""
				rv=$((rv - 1))
			done
			shift $((havefiles))
		fi ;;
	esac
	if [ "$M" = per_file_offsets ] && [ $# -gt 1 ]; then
		rcmax=0
		: >"$t.2"
		for f; do
			command od $RA "$f" >"$t" 2>>"$e"
			r=$?
			[ $r -gt $rcmax ] && rcmax=$r
			cat "$t" >>"$t.2"
		done
		cat "$t.2"
		cat "$e" >&2
		exit $rcmax
	fi
	if [ "$M" = t_grouped ] && [ $ntok -gt 1 ]; then
		: >"$t.2"
		rcmax=0
		for tk in $tlist; do
			fixtok "$tk"
			a1="-A n -v"
			[ -n "$A" ] && a1="-A $A -v"
			command od $a1 $JN -t $tok ${1+"$@"} >"$t" 2>"$e"
			r=$?
			[ $r -gt $rcmax ] && rcmax=$r
			cat "$t" >>"$t.2"
		done
		cat "$t.2"
		exit $rcmax
	fi

	# the size and padding of the last item
	sizefault=
	case $M in
	partial_dropped | partial_ff | endian_swap) sizefault=$M ;;
	esac
	if [ -n "$sizefault" ]; then
		avail=$(total ${1+"$@"})
		if [ $# -eq 0 ]; then
			cat >"$t.1"
			set -- "$t.1"
			avail=$(total "$t.1")
		fi
		skip=0
		[ -n "$jv" ] && skip=$(dec "$jv")
		avail=$((avail - skip))
		[ -n "$nv" ] && [ "$(dec "$nv")" -lt $avail ] && avail=$(dec "$nv")
		[ $avail -lt 0 ] && avail=0
		szz=${szt:-1}
		case $szz in [0-9]*) ;; *) szz=1 ;; esac
		case $sizefault in
		partial_dropped)
			cat -- "$@" | tail -c +$((skip + 1)) | head -c $((avail / szz * szz)) >"$t.2" ;;
		partial_ff)
			{
				cat -- "$@" | tail -c +$((skip + 1)) | head -c $avail
				p=$(((szz - avail % szz) % szz))
				while [ $p -gt 0 ]; do printf '\377'; p=$((p - 1)); done
			} >"$t.2" ;;
		endian_swap)
			p=$(((szz - avail % szz) % szz))
			ob=$(cat -- "$@" | tail -c +$((skip + 1)) | head -c $avail | command od -A n -v -t o1)
			esc=$(printf '%s\n' "$ob" | awk -v sz="$szz" -v pad="$p" '
				{ for (i = 1; i <= NF; i++) b[n++] = $i }
				END {
					for (i = 0; i < pad; i++) b[n++] = "000"
					for (i = 0; i < n; i += sz)
						for (j = i + sz - 1; j >= i; j--) printf "\\%s", b[j]
				}')
			printf "$esc" >"$t.2" ;;
		esac
		RA2=
		[ -n "$A" ] && RA2="-A $A"
		RA2="$RA2$RT"
		[ -n "$vf" ] && RA2="$RA2 -v"
		command od $RA2 <"$t.2" >"$t" 2>"$e"
		rc=$?
		cat "$t"
		cat "$e" >&2
		exit $rc
	fi

	# the input faults
	case $M in
	reads_stdin_too)
		if [ $havefiles -gt 0 ]; then set -- "$@" -; fi ;;
	j_per_file)
		if [ -n "$jv" ] && [ $# -gt 0 ]; then
			: >"$t.1"
			for f; do tail -c +$(($(dec "$jv") + 1)) "$f" >>"$t.1"; done
			JN2=
			[ -n "$nv" ] && JN2="-N $nv"
			command od ${A:+-A $A}$RT${vf:+ -v} $JN2 <"$t.1" >"$t" 2>"$e"
			rc=$?
			RUN_DONE=1
		fi ;;
	N_per_file)
		if [ -n "$nv" ] && [ $# -gt 0 ]; then
			: >"$t.1"
			for f; do head -c "$(dec "$nv")" "$f" >>"$t.1"; done
			JN2=
			[ -n "$jv" ] && JN2="-j $jv"
			command od ${A:+-A $A}$RT${vf:+ -v} $JN2 <"$t.1" >"$t" 2>"$e"
			rc=$?
			RUN_DONE=1
		fi ;;
	esac
	if [ -z "${RUN_DONE:-}" ]; then
		command od $RA ${1+"$@"} >"$t" 2>"$e"
		rc=$?
	fi

	# the status faults
	case $M in
	status0_missing) [ $rc -ne 0 ] && [ -z "$jv" ] && rc=0 ;;
	status0_partial) [ $rc -ne 0 ] && [ -s "$t" ] && rc=0 ;;
	j_short_status0) [ $rc -ne 0 ] && [ -n "$jv" ] && rc=0 ;;
	j_short_silent) [ $rc -ne 0 ] && [ -n "$jv" ] && : >"$e" ;;
	j_short_stdout) [ $rc -ne 0 ] && [ -n "$jv" ] && { cat "$e" >>"$t"; : >"$e"; } ;;
	N_short_err)
		if [ -n "$nv" ] && [ $# -gt 0 ]; then
			av=$(total "$@")
			[ -n "$jv" ] && av=$((av - $(dec "$jv")))
			if [ "$av" -lt "$(dec "$nv")" ]; then rc=1; echo "od: short" >&2; fi
		fi ;;
	j_equal_err)
		if [ -n "$jv" ] && [ $# -gt 0 ] && [ "$(total "$@")" = "$(dec "$jv")" ]; then rc=1; echo "od: skip" >&2; fi ;;
	j_limit_plus1)
		if [ -n "$jv" ] && [ $# -gt 0 ] && [ "$(dec "$jv")" -eq $(($(total "$@") + 1)) ]; then rc=0; : >"$e"; fi ;;
	status2) [ $rc -ne 0 ] && rc=2 ;;
	status255) [ $rc -ne 0 ] && rc=255 ;;
	esac

	# --- the output
	ob() { # the offset base of the listing
		echo "${A:-o}"
	}
	case $M in
	off_final_missing)
		[ "$A" = n ] || awk '{ l[NR] = $0 } END { for (i = 1; i <= NR; i++) if (!(i == NR && split(l[i], f, " ") == 1)) print l[i] }' "$t" >"$t.2"
		[ -f "$t.2" ] && cat "$t.2" >"$t" ;;
	off_final_blank)
		[ "$A" = n ] || awk '{ l[NR] = $0 } END { for (i = 1; i <= NR; i++) print l[i] (i == NR && split(l[i], f, " ") == 1 ? " " : "") }' "$t" >"$t.2"
		[ -f "$t.2" ] && cat "$t.2" >"$t" ;;
	off_final_plus1 | off_all_plus1)
		if [ "$A" != n ]; then
			: >"$t.2"
			nl=$(awk 'END { print NR }' "$t")
			i=0
			while IFS= read -r line; do
				i=$((i + 1))
				case $line in
				[0-9a-fA-F]*)
					if [ "$M" = off_all_plus1 ] || [ $i -eq "$nl" ]; then
						off=${line%%[ 	]*}
						rest=${line#"$off"}
						case ${A:-o} in
						d) z=$off; while :; do case $z in 0?*) z=${z#0} ;; *) break ;; esac; done; v=$(($z + 1)); new=$(printf '%07d' "$v") ;;
						o) v=$((0$off + 1)); new=$(printf '%07o' "$v") ;;
						x) v=$((0x$off + 1)); new=$(printf '%07x' "$v") ;;
						esac
						line=$new$rest
					fi ;;
				esac
				printf '%s\n' "$line" >>"$t.2"
			done <"$t"
			cat "$t.2" >"$t"
		fi ;;
	off_lead_blank)
		[ "$A" = n ] || sed 's/^\([^ 	]\)/ \1/' "$t" >"$t.2"
		[ -f "$t.2" ] && cat "$t.2" >"$t" ;;
	off_glued)
		[ "$A" = n ] || awk '{ if ($0 ~ /^[^ \t]/ && NF > 1) { s = $1; $1 = ""; sub(/^ +/, ""); print s $0 } else print }' "$t" >"$t.2"
		[ -f "$t.2" ] && cat "$t.2" >"$t" ;;
	off_zero)
		[ "$A" = n ] || awk '{ if ($0 ~ /^[^ \t]/) { sub(/^[^ \t]+/, "0000000") } print }' "$t" >"$t.2"
		[ -f "$t.2" ] && cat "$t.2" >"$t" ;;
	off_plain)
		# permitted: offsets without leading zeros
		[ "$A" = n ] || awk '{ if ($0 ~ /^[^ \t]/ && $0 != "*") { match($0, /^[^ \t]+/); o = substr($0, 1, RLENGTH); r = substr($0, RLENGTH + 1); while (length(o) > 1 && substr(o, 1, 1) == "0") o = substr(o, 2); print o r } else print }' "$t" >"$t.2"
		[ -f "$t.2" ] && cat "$t.2" >"$t" ;;
	off_wide)
		[ "$A" = n ] || awk '{ if ($0 ~ /^[^ \t]/ && $0 != "*") { match($0, /^[^ \t]+/); o = substr($0, 1, RLENGTH); r = substr($0, RLENGTH + 1); while (length(o) < 12) o = "0" o; print o r } else print }' "$t" >"$t.2"
		[ -f "$t.2" ] && cat "$t.2" >"$t" ;;
	gap3)
		awk 'BEGIN { OFS = "   " } { if (NF) { $1 = $1; print } else print }' "$t" >"$t.2"
		cat "$t.2" >"$t" ;;
	trail_blank)
		awk -v A="$A" '{ if (A != "n" && NF == 1 && $0 ~ /^[^ \t]/) print; else if (NF) print $0 "   "; else print }' "$t" >"$t.2"
		cat "$t.2" >"$t" ;;
	an_empty)
		[ "$A" = n ] && echo >>"$t" ;;
	hex_upper)
		allx=1
		for tk in $tlist; do case $tk in x*) ;; *) allx= ;; esac; done
		if [ -n "$allx" ] && [ -n "$tlist" ]; then
			tr 'a-f' 'A-F' <"$t" >"$t.2"
			cat "$t.2" >"$t"
		fi ;;
	pad_none)
		if [ -n "$allint" ]; then
			awk -v A="$A" '{ for (i = (A == "n" ? 1 : ($0 ~ /^[^ \t]/ ? 2 : 1)); i <= NF; i++) { s = $i; while (length(s) > 1 && substr(s, 1, 1) == "0") s = substr(s, 2); $i = s } ; if (NF) print; else print }' "$t" >"$t.2"
			cat "$t.2" >"$t"
		fi ;;
	esac
	if [ -n "$dedup" ]; then
		awk -v M="$M" '
			{ L[NR] = $0 }
			END {
				last = NR
				ng = 0
				for (i = 1; i < last; i++) {
					if (L[i] ~ /^[^ \t]/) { ng++; G[ng] = L[i]; k = L[i]; sub(/^[^ \t]+/, "", k); K[ng] = k }
					else { G[ng] = G[ng] "\n" L[i]; K[ng] = K[ng] "\n" L[i] }
				}
				star = (M == "star_blank") ? "* " : "*"
				instar = 0
				for (g = 1; g <= ng; g++) {
					dup = (g > 1 && K[g] == K[g-1])
					if (M == "star_cmp_first") dup = (g > 1 && K[g] == K[1])
					if (M == "star_first_token") { split(K[g], a, " "); split(K[g-1], b, " "); dup = (g > 1 && a[1] == b[1]) }
					if (M == "star_first_replaced" && g == 1 && ng > 1 && K[2] == K[1]) dup = 1
					if (dup) { if (!instar) print star; instar = 1 }
					else { print G[g]; instar = 0 }
				}
				print L[last]
			}' "$t" >"$t.2"
		cat "$t.2" >"$t"
	fi
	case $M in
	block8 | block32)
		# permitted: another block size (single one-byte type only)
		W=8
		[ "$M" = block32 ] && W=32
		if [ $ntok -le 1 ]; then
			case ${tlist:-oS} in
			a | x1 | o1 | u1 | d1) ;;
			*) W= ;;
			esac
		else
			W=
		fi
		if [ -n "$W" ]; then
			a1=n
			startoff=0
			[ -n "$jv" ] && startoff=$(dec "$jv")
			command od -A n -v $JN $RT ${1+"$@"} >"$t.1" 2>"$e"
			rc=$?
			awk -v W="$W" -v A="${A:-o}" -v V="$vf" -v S="$startoff" '
				{ for (i = 1; i <= NF; i++) tok[n++] = $i }
				END {
					fmt = (A == "d") ? "%07d" : (A == "o") ? "%07o" : "%07x"
					off = S; instar = 0; prev = ""
					for (i = 0; i < n; i += W) {
						line = ""; cnt = 0
						for (j = i; j < i + W && j < n; j++) { line = line (cnt ? " " : "") tok[j]; cnt++ }
						if (!V && i > 0 && line == prev) {
							if (!instar) print "*"
							instar = 1
						} else {
							if (A != "n") printf fmt " ", off
							print line
							instar = 0
						}
						prev = line
						off += cnt
					}
					if (A != "n") printf fmt "\n", off
				}' "$t.1" >"$t"
		fi ;;
	esac
	if [ -n "$fmtkind" ]; then
		awk -v M="$M" -v K="$fmtkind" '
			function oct(s,   i, v) { v = 0; for (i = 1; i <= length(s); i++) v = v * 8 + substr(s, i, 1); return v }
			BEGIN {
				split("nul soh stx etx eot enq ack bel bs ht lf vt ff cr so si dle dc1 dc2 dc3 dc4 nak syn etb can em sub esc fs gs rs us", nm, " ")
				bs = sprintf("%c", 92)
			}
			function aname(v,   s) {
				if (M != "a_high_raw" && M != "a_mask6") v = v % 128
				if (M == "a_mask6") v = v % 64
				if (v < 32) {
					s = nm[v + 1]
					if (v == 10 && M != "lf_name") s = "nl"
					if (M == "a_bel_bell" && v == 7) s = "bell"
					if (M == "a_ht_tab" && v == 9) s = "tab"
					if (M == "a_esc_upper" && v == 27) s = "ESC"
					if (M == "a_upper") s = toupper(s)
					if (M == "a_nul_zero" && v == 0) s = "0"
					if (M == "a_soh_octal" && v == 1) s = "001"
					if (M == "a_vt_ff" && v == 11) s = "ff"
					if (M == "a_vt_ff" && v == 12) s = "vt"
					if (M == "a_us_blank" && v == 31) s = "unit"
					if (M == "a_nl_lf_both" && v == 10) s = "nl lf"
					return s
				}
				if (v == 32) { if (M == "a_sp_name") return "space"; if (M == "a_sp_underscore") return "_"; return "sp" }
				if (v == 127) { if (M == "a_del_upper") return "DEL"; if (M == "a_del_octal") return "177"; return "del" }
				if (v >= 128) return sprintf("%03o", v)
				return sprintf("%c", v)
			}
			function cname(v,   s) {
				if (v == 0) return (M == "c_nul_oct") ? "000" : (M == "c_nul_bsbs") ? "\\\\0" : "\\0"
				if (v == 7) return (M == "c_a_oct") ? "007" : "\\a"
				if (v == 8) return (M == "c_b_oct") ? "010" : "\\b"
				if (v == 9) return (M == "c_t_oct") ? "011" : "\\t"
				if (v == 10) return (M == "c_n_oct") ? "012" : "\\n"
				if (v == 11) return (M == "c_v_oct") ? "013" : "\\v"
				if (v == 12) return (M == "c_f_oct") ? "014" : "\\f"
				if (v == 13) return (M == "c_r_oct") ? "015" : "\\r"
				if (v == 27 && M == "c_esc_e") return "\\e"
				if (v == 92) return (M == "c_bs_double") ? "\\\\" : (M == "c_bs_octal") ? "134" : bs
				if (v == 32) return (M == "c_space_oct") ? "040" : " "
				if (v == 127 && M == "c_del_raw") return "DEL"
				if (v > 32 && v < 127) {
					if (M == "c_printable_oct" && v >= 97 && v <= 122) return sprintf("%03o", v)
					return sprintf("%c", v)
				}
				if (M == "c_ctl_short") return sprintf("%02o", v)
				if (M == "c_ctl_hex") return sprintf("x%02x", v)
				if (M == "c_ctl_long") return sprintf("%04o", v)
				return sprintf("%03o", v)
			}
			{ for (i = 1; i <= NF; i++) { v = oct($i); tok[n++] = (K == "a") ? aname(v) : cname(v) } }
			END {
				for (i = 0; i < n; i += 16) {
					line = ""
					for (j = i; j < i + 16 && j < n; j++) line = line sprintf("%5s", tok[j])
					print line
				}
			}' "$t" >"$t.2"
		cat "$t.2" >"$t"
	fi
	cat "$t"
	[ -s "$e" ] && cat "$e" >&2
	exit $rc
)

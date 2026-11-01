# Shared by the nm faults and variants: a wrapper over the real nm that
# takes its symbol information from the real utility and plants one fault
# (or one permitted choice), chosen by name. Written for the portable format.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# pc_nmref NAME ARGS...: behaves as nm should, except for what NAME says.
# The options -e, -f, -x and -o are understood here whether or not the real
# utility has them (-e and -f are dropped, -x and -o become -t x and -t o).
pc_nmref() (
	M=$1
	shift
	A= P= V= G= U= T= E= F= X= O= T0=
	if [ "$M" = t_attached_bad ]; then
		for a in "$@"; do
			case $a in -t?* | -?*t?*) echo "nm: invalid option" >&2; exit 1 ;; esac
		done
	fi
	OPTIND=1
	while getopts APvgut:efox o; do
		case $o in
		A) A=1 ;;
		P) P=1 ;;
		v) V=1 ;;
		g) G=1 ;;
		u) U=1 ;;
		t) T=$OPTARG ;;
		e) E=1 ;;
		f) F=1 ;;
		x) T=x X=1 ;;
		o) T=o O=1 ;;
		*) echo "nm: invalid option" >&2; exit 1 ;;
		esac
	done
	shift $((OPTIND - 1))
	nops=$#
	first=$1
	last=
	for a in "$@"; do last=$a; done
	# a fault named for an option acts only when that option is given
	case $M in
	u_*) [ -n "$U" ] || M=none ;;
	g_*) [ -n "$G" ] || M=none ;;
	e_*) [ -n "$E" ] || M=none ;;
	f_*) [ -n "$F" ] || M=none ;;
	x_*) [ -n "$X" ] || M=none ;;
	o_*) [ -n "$O" ] || M=none ;;
	A_*) [ -n "$A" ] || M=none ;;
	h_*) [ -z "$A" ] || M=none ;;
	v_*) [ -n "$V" ] || M=none ;;
	esac
	oV=$V oG=$G oU=$U oA=$A
	# faults that change how the options are understood
	case $M in
	u_ignored | u_all | u_inverted | u_adds_text) U= ;;
	u_as_g) U= G=1 ;;
	g_ignored | g_inverted | g_adds_local_data | g_adds_local_text) G= ;;
	v_ignored | v_text_order | v_descending | v_by_size) V= ;;
	A_ignored | A_headers_too) [ "$M" = A_ignored ] && A= ;;
	t_d_as_hex) [ "$T" = d ] && T=x ;;
	t_x_dec) [ "$T" = x ] && T=d ;;
	t_o_dec) [ "$T" = o ] && T=d ;;
	t_o_as_hex) [ "$T" = o ] && T=x ;;
	t_ignored) T= ;;
	t_pdefault_dec) [ -z "$T" ] && [ -n "$P" ] && T=d ;;
	x_dec) [ -n "$X" ] && T=d ;;
	x_as_o) [ -n "$X" ] && T=o ;;
	o_dec) [ -n "$O" ] && T=d ;;
	o_as_x) [ -n "$O" ] && T=x ;;
	P_ignored) P= ;;
	size_trunc32) T0=${T:-x}; [ -z "$P" ] && T0=${T:-d}; T=d ;;
	miss_abort)
		keep=
		for a in "$@"; do
			if [ ! -e "$a" ]; then
				echo "nm: '$a': No such file" >&2
				break
			fi
			keep="$keep $a"
		done
		set -- $keep
		;;
	esac
	ofl=
	[ -n "$A" ] && ofl="$ofl -A"
	[ -n "$P" ] && ofl="$ofl -P"
	[ -n "$V" ] && ofl="$ofl -v"
	[ -n "$G" ] && ofl="$ofl -g"
	[ -n "$U" ] && ofl="$ofl -u"
	[ -n "$T" ] && ofl="$ofl -t $T"
	t=pc-nmref.$$
	trap 'rm -f "$t" "$t".*' EXIT
	if [ "$#" -eq 0 ] && [ "$nops" -ne 0 ]; then
		: >"$t"
		: >"$t.e"
		rc=1
	else
		command nm $ofl "$@" >"$t" 2>"$t.e"
		rc=$?
	fi
	[ "$M" = miss_abort ] && [ "$rc" -eq 0 ] && [ "$#" -ne "$nops" ] && rc=1
	b=0
	[ -n "$A" ] && b=1
	case $M in
	t_size_dec | t_value_dec)
		command nm ${A:+-A} ${P:+-P} ${V:+-v} ${G:+-g} ${U:+-u} -t d "$@" >"$t.d" 2>/dev/null
		if [ "$M" = t_size_dec ]; then c=$((4 + b)); else c=$((3 + b)); fi
		awk -v c="$c" 'NR == FNR { d[NR] = $c; next }
			NF >= c { $c = d[FNR] } { print }' "$t.d" "$t" >"$t.m" && mv "$t.m" "$t"
		;;
	esac
	# the output filter
	awk -v M="$M" -v b="$b" -v T="$T" -v E="$E" -v F="$F" -v oU="$oU" -v oG="$oG" \
		-v nops="$nops" -v first="$first" -v pwdv="$PWD" -v hasA="$oA" -v T0="$T0" '
	function conv(n, base,   r, d) {
		if (n == 0) return "0"
		r = ""
		while (n > 0) { d = n % base; r = substr("0123456789abcdef", d + 1, 1) r; n = (n - d) / base }
		return r
	}
	function lower(t) { return t ~ /^[a-z]$/ }
	function dbg(t) { return t == "D" || t == "B" || t == "C" }
	function isdata(t) { return t ~ /^[DBCdbc]$/ }
	function istext(t) { return t == "T" || t == "t" }
	{ L[++n] = $0 }
	END {
		sep = " "
		if (M == "padded") sep = "   "
		if (M == "tabbed") sep = "\t"
		nh = 0; lastpre = ""; kept = 0; firstmem = ""; firstpre = ""
		if (M == "h_single" && nops == 1 && !hasA) print first ":"
		for (i = 1; i <= n; i++) {
			line = L[i]
			k = split(line, f, " ")
			if (k == 1 && f[1] ~ /:$/ && !hasA) {
				nh++
				hd[nh] = line
				hl[nh] = i
				continue
			}
		}
		lasth = nh
		hcount = 0
		for (i = 1; i <= n; i++) {
			line = L[i]
			k = split(line, f, " ")
			if (k == 0) { print line; continue }
			if (k == 1 && f[1] ~ /:$/ && !hasA) {
				hcount++
				h = line
				if (M == "h_none") continue
				if (M == "h_first_only" && hcount > 1) continue
				if (M == "h_not_last" && hcount == lasth) continue
				if (M == "h_no_colon") sub(/:$/, "", h)
				if (M == "h_lib_no_member") sub(/\[[^]]*\]/, "", h)
				if (M == "h_lib_paren") { sub(/\[/, "(", h); sub(/\]/, ")", h) }
				if (M == "h_first_name") h = hd[1]
				if (M == "h_after") {
					# print the header after the lines that follow it
					for (j = i + 1; j <= n; j++) {
						kk = split(L[j], g, " ")
						if (kk == 1 && g[1] ~ /:$/) break
						print L[j]
						skip[j] = 1
					}
					print h
					continue
				}
				print h
				continue
			}
			if (skip[i]) continue
			if (k < 2 + b) { print line; continue }
			pre = b ? f[1] : ""
			name = f[1 + b]; type = f[2 + b]; val = f[3 + b]; sz = f[4 + b]
			hasv = (k >= 3 + b); hass = (k >= 4 + b)
			# line selection
			if (M == "u_inverted" && type == "U") continue
			if (M == "u_adds_text" && type != "U" && type != "T") continue
			if (M == "u_none" || M == "g_none" || M == "e_none" && E || M == "f_none" && F || M == "nothing") continue
			if (M == "u_first_only" && ++kept > 1) continue
			if (M == "g_inverted" && !lower(type)) continue
			if (M == "g_adds_local_data" && lower(type) && type != "d") continue
			if (M == "g_adds_local_text" && lower(type) && type != "t") continue
			if (M == "g_drops_data" && dbg(type)) continue
			if (M == "g_drops_bss" && (type == "B" || type == "C")) continue
			if (M == "g_drops_text" && type == "T") continue
			if (M == "g_first_only" && ++kept > 1) continue
			if (E != "") {
				if (M == "e_drops_local" && lower(type)) continue
				if (M == "e_drops_global" && !lower(type) && type != "U") continue
				if (M == "e_drops_data" && dbg(type)) continue
				if (M == "e_drops_text" && type == "T") continue
				if (M == "e_drops_static_data" && (type == "d" || type == "b")) continue
				if (M == "e_first_only" && ++kept > 1) continue
			}
			if (F != "") {
				if (M == "f_drops_local" && lower(type)) continue
				if (M == "f_drops_undef" && type == "U") continue
				if (M == "f_drops_data" && isdata(type)) continue
				if (M == "f_drops_text" && istext(type)) continue
				if (M == "f_first_only" && ++kept > 1) continue
			}
			# line changes
			if (M == "fn_lower" && type == "T") type = "t"
			if (M == "fn_D" && type == "T") type = "D"
			if (M == "data_lower" && type == "D") type = "d"
			if (M == "data_B" && type == "D") type = "B"
			if (M == "undef_lower" && type == "U") type = "u"
			if (M == "undef_T" && type == "U") type = "T"
			if (M == "name_trunc" && length(name) > 32) name = substr(name, 1, 32)
			if (M == "t_x_upper") { val = toupper(val); sz = toupper(sz) }
			if (M == "t_x_0x" && hasv && type != "U") { val = "0x" val; sz = "0x" sz }
			if (M == "t_o_zero" && T == "o" && hasv && type != "U") { val = "0" val; sz = "0" sz }
			if (M == "size_trunc32" && hass) {
				x = (sz + 0) % 4294967296
				sz = conv(x, T0 == "o" ? 8 : (T0 == "x" ? 16 : 10))
			}
			if (M == "size_trunc32" && hasv && type != "U") val = conv(val + 0, T0 == "o" ? 8 : (T0 == "x" ? 16 : 10))
			if (M == "prefixed") name = "_" name
			if (M == "common_C" && type == "B") type = "C"
			if (M == "undef_zero" && type == "U" && !hasv) { val = "0"; sz = "0"; hasv = 1; hass = 1 }
			if (M == "impl_local_types" && lower(type)) type = "L"
			if (M == "size_zero" && hass) sz = "0"
			if (M == "f_default_format" && F != "") { print val, type, name; continue }
			s = name sep type
			if (M == "name_type_swapped") s = type sep name
			if (hasv && M == "value_size_swapped" && type != "U" && hass) { t0 = val; val = sz; sz = t0 }
			if (hasv) s = s sep val
			if (hass && M != "size_dropped" || M == "size_dropped" && type == "U" && hass) s = s sep sz
			if (M == "extra_field") s = s sep "extra"
			# prefix (with -A)
			if (b) {
				if (firstpre == "") { firstpre = pre; firstmem = pre; sub(/^[^[]*\[/, "", firstmem); sub(/\].*$/, "", firstmem) }
				if (M == "A_no_colon") sub(/:$/, "", pre)
				if (M == "A_basename") sub(/^.*\//, "", pre)
				if (M == "A_absolute" && pre !~ /^\//) pre = pwdv "/" pre
				if (M == "A_first_file") pre = firstpre
				if (M == "A_lib_no_member") sub(/\[[^]]*\]/, "", pre)
				if (M == "A_lib_paren") { sub(/\[/, "(", pre); sub(/\]/, ")", pre) }
				if (M == "A_lib_first_member") { m = pre; sub(/^[^[]*\[/, "", m); sub(/\].*$/, "", m); if (m != pre) sub("\\[" m "\\]", "[" firstmem "]", pre) }
				if (M == "A_headers_too" && pre != lastpre) print pre
				lastpre = pre
				if (M == "A_first_line_only" && i > 1) { print s; continue }
				if (M == "A_undef_bare" && type == "U") { print s; continue }
				if (M == "A_no_blank") { print pre s; continue }
				if (M == "A_tab") { print pre "\t" s; continue }
				if (M == "padded") { print pre "    " s; continue }
				s = pre " " s
			}
			print s
			if (M == "f_extra" && F != "" && !fdone) { fdone = 1; print ".text" sep "t" sep "0" sep "0"; print ".data" sep "d" sep "0" sep "0"; print ".bss" sep "b" sep "0" sep "0" }
		}
	}' "$t" >"$t.o"
	# ordering faults and variants
	case $M in
	v_text_order) sort -s -k"$((3 + b))","$((3 + b))" "$t.o" >"$t.s" ;;
	v_descending) sort -s -n -r -k"$((3 + b))","$((3 + b))" "$t.o" >"$t.s" ;;
	v_by_size) sort -s -n -k"$((4 + b))","$((4 + b))" "$t.o" >"$t.s" ;;
	default_by_value) if [ -z "$oV" ]; then sort -s -n -k"$((3 + b))","$((3 + b))" "$t.o" >"$t.s"; else cp "$t.o" "$t.s"; fi ;;
	default_name_desc) if [ -z "$oV" ]; then sort -r -k"$((1 + b))","$((1 + b))" "$t.o" >"$t.s"; else cp "$t.o" "$t.s"; fi ;;
	v_ties_reversed) if [ -n "$oV" ]; then sort -k"$((3 + b))","$((3 + b))n" -k"$((1 + b))","$((1 + b))r" "$t.o" >"$t.s"; else cp "$t.o" "$t.s"; fi ;;
	v_undef_first) if [ -n "$oV" ]; then { awk -v b="$b" '$(2 + b) == "U"' "$t.o"; awk -v b="$b" '$(2 + b) != "U"' "$t.o"; } >"$t.s"; else cp "$t.o" "$t.s"; fi ;;
	*) cp "$t.o" "$t.s" ;;
	esac
	# status and streams
	case $M in
	status1) [ "$rc" -eq 0 ] && rc=1 ;;
	status2) [ "$rc" -ne 0 ] && rc=2 ;;
	status255) [ "$rc" -ne 0 ] && rc=255 ;;
	miss_status0) rc=0 ;;
	miss_status_last) [ -e "$last" ] && rc=0 ;;
	miss_status_first) [ -e "$first" ] && rc=0 ;;
	miss_no_diag) : >"$t.e" ;;
	miss_diag_stdout) if [ "$rc" -ne 0 ]; then cat "$t.e"; : >"$t.e"; fi ;;
	miss_discard) [ "$rc" -ne 0 ] && : >"$t.s" ;;
	noise) echo "nm: warning: something" >&2 ;;
	nosym_status1) if [ "$rc" -eq 0 ] && ! grep -v ':$' "$t.s" >/dev/null 2>&1; then echo "nm: $first: no symbols" >&2; rc=1; fi ;;
	nosym_stdout) if ! grep -v ':$' "$t.s" >/dev/null 2>&1; then echo "nm: $first: no symbols"; fi ;;
	nosym_stderr) if ! grep -v ':$' "$t.s" >/dev/null 2>&1; then echo "nm: $first: no symbols" >&2; fi ;;
	esac
	if [ "$M" = out_stderr ]; then cat "$t.s" >&2; else cat "$t.s"; fi
	cat "$t.e" >&2
	exit "$rc"
)

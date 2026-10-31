# A reference tr for the POSIX locale with planted faults and variants.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# Input: the decimal byte values of standard input (od -tu1). The strings
# come in the environment (S1, S2), so that backslashes survive. M names the
# fault or variant; the default behaviour is a conforming tr that leaves
# characters without a counterpart in a short string2 unchanged.

function esccode(ch) {
	if (ch == "a") return 7
	if (ch == "b") return 8
	if (ch == "f") return 12
	if (ch == "n") return 10
	if (ch == "r") return 13
	if (ch == "t") return 9
	if (ch == "v") return 11
	if (ch == "\\") return 92
	return -1
}

# elem(str, i): one character, octal sequence or backslash escape at i;
# sets E_code, E_kind (p plain, o octal, e escape) and E_next.
function elem(str, i,    ch, nx, v, j, d, mx, base, c, len) {
	len = length(str)
	ch = substr(str, i, 1)
	if (ch == "\\" && i < len) {
		nx = substr(str, i + 1, 1)
		if (index("01234567", nx)) {
			mx = 3
			if (M == "oct_one_digit") mx = 1
			if (M == "oct_long") mx = 4
			base = 8
			if (M == "oct_decimal") base = 10
			v = 0; j = i + 1; d = 0
			while (j <= len && d < mx && index("01234567", substr(str, j, 1))) {
				v = v * base + substr(str, j, 1)
				j++; d++
			}
			if (M == "oct_need3" && d < 3) {
				E_code = ord["\\"]; E_kind = "p"; E_next = i + 1
				return
			}
			E_code = v % 256; E_kind = "o"; E_next = j
			return
		}
		c = esccode(nx)
		if (c >= 0) {
			if (M == "esc_" nx) c = ord[nx]
			E_code = c; E_kind = "e"; E_next = i + 2
			return
		}
		E_code = ord[nx]; E_kind = "p"; E_next = i + 2
		return
	}
	E_code = ord[ch]; E_kind = "p"; E_next = i + 1
}

# tokenize(str, which, code, kind, nm, cnt): kinds are p, o, e (a character),
# c (class, name in nm) and r ([x*n], count text in cnt).
function tokenize(str, which, code, kind, nm, cnt,    n, i, len, ch, p, j, k, d) {
	n = 0; i = 1; len = length(str)
	while (i <= len) {
		ch = substr(str, i, 1)
		if (ch == "[" && substr(str, i + 1, 1) == ":" && !(M == "s2_class_literal" && which == 2 && DF != "" && SF != "")) {
			p = index(substr(str, i + 2), ":]")
			if (p > 0) {
				n++; kind[n] = "c"; nm[n] = substr(str, i + 2, p - 1)
				i = i + 2 + p + 1
				continue
			}
		}
		if (ch == "[" && which == 2) {
			elem(str, i + 1)
			j = E_next
			if (substr(str, j, 1) == "*") {
				k = j + 1; d = ""
				while (index("0123456789", substr(str, k, 1)) && k <= len) { d = d substr(str, k, 1); k++ }
				if (substr(str, k, 1) == "]") {
					n++; kind[n] = "r"; code[n] = E_code; cnt[n] = d
					if (M == "rep_x_escape_raw" && E_kind != "p") code[n] = ord["n"]
					i = k + 1
					continue
				}
			}
		}
		elem(str, i)
		n++; code[n] = E_code; kind[n] = E_kind
		i = E_next
		if (M == "bs_bs_n" && E_kind == "e" && E_code == 92 && substr(str, i, 1) == "n") {
			n++; code[n] = 10; kind[n] = "e"; i++
		}
	}
	return n
}

function isclass(nm, c,    up, lo, di) {
	up = (c >= 65 && c <= (M == "upper_nz" ? 89 : 90))
	lo = (c >= 97 && c <= (M == "lower_nz" ? 121 : 122))
	di = (c >= 48 && c <= 57)
	if (nm == "upper") return up
	if (nm == "lower") return lo
	if (nm == "alpha") return (M == "alpha_nolower") ? up : (up || lo)
	if (nm == "digit") return di || (M == "digit_hex" && ((c >= 65 && c <= 70) || (c >= 97 && c <= 102)))
	if (nm == "alnum") return (M == "alnum_nodigit") ? (up || lo) : (up || lo || di)
	if (nm == "xdigit") return di || (c >= 65 && c <= 70) || (M != "xdigit_nolower" && c >= 97 && c <= 102)
	if (nm == "space") {
		if (c == 32) return M != "space_no_blank"
		if (c == 9) return M != "space_no_tab"
		if (c == 10) return M != "space_no_nl"
		if (c == 11) return M != "space_no_vt"
		if (c == 12) return M != "space_no_ff"
		if (c == 13) return M != "space_no_cr"
		return 0
	}
	if (nm == "blank") return c == 32 || c == 9 || (M == "blank_nl" && c == 10)
	if (nm == "punct") return c >= 33 && c <= 126 && !(up || lo || di) && !(M == "punct_nounder" && c == 95)
	if (nm == "cntrl") return c < 32 || (c == 127 && M != "cntrl_nodel")
	if (nm == "graph") return (c >= 33 && c <= 126) || (M == "graph_space" && c == 32)
	if (nm == "print") return c >= 32 && c <= 126 && !(M == "print_nospace" && c == 32)
	return 0
}

# expand(...): the array for a string; fill is the number of characters a
# [x*] stands for. Returns the length. conv is set for a case conversion.
function expand(code, kind, nm, cnt, n, out, fill, conv,    j, o, lo, hi, c, k, m, d, v) {
	o = 0
	for (j = 1; j <= n; j++) {
		if (kind[j] == "c") {
			m = 0
			if (M == "class_reverse" && !conv) {
				for (c = 255; c >= 0; c--) if (isclass(nm[j], c)) out[++o] = c
			} else {
				for (c = 0; c <= 255; c++) if (isclass(nm[j], c)) {
					if (conv && M == "case_partial" && m >= 13) continue
					out[++o] = c; m++
				}
			}
		} else if (kind[j] == "r") {
			d = cnt[j]
			if (d == "") v = fill
			else if (substr(d, 1, 1) == "0" && M != "rep_octal_dec") {
				v = 0
				for (k = 1; k <= length(d); k++) v = v * 8 + substr(d, k, 1)
				if (v == 0 && M != "rep_zero_empty") v = fill
			} else {
				if (M == "rep_firstdigit") d = substr(d, 1, 1)
				v = d + 0
				if (v == 0 && M != "rep_zero_empty") v = fill
			}
			if (cnt[j] == "" && M == "fill_one") v = 1
			if (cnt[j] == "" && M == "fill_to_end") { v = FILLTOTAL - o; if (v < 0) v = 0 }
			if (cnt[j] != "" && M == "rep_off") v++
			if (cnt[j] != "" && M == "rep_one") v = 1
			for (k = 1; k <= v; k++) out[++o] = code[j]
		} else if (j + 2 <= n && kind[j + 1] == "p" && code[j + 1] == 45 && kind[j + 2] != "c" && kind[j + 2] != "r" \
			&& M != "range_literal" && !(M == "oct_range_literal" && (kind[j] == "o" || kind[j + 2] == "o"))) {
			lo = code[j]; hi = code[j + 2]
			if (M == "range_excl") hi--
			if (M == "range_start_excl") lo++
			for (c = lo; c <= hi; c++) out[++o] = c
			j += 2
		} else {
			out[++o] = code[j]
		}
	}
	return o
}

function stash(v) {
	if (M == "large_truncate" && no >= 4096) return
	out[no++] = v
}

function proc(v,    orig, del, w, insq, eq) {
	if (stopped) return
	if (M == "nul_stops" && v == 0) { stopped = 1; return }
	if (M == "nul_dropped" && v == 0) return
	orig = v
	if (DF != "") {
		del = (v in dset)
		if (M == "d_inverse") del = !del
		if (M == "d_first_only" && del) { if (v in seen) del = 0; else seen[v] = 1 }
		if (M == "ds_no_delete" && SF != "") del = 0
		if (M == "ds_swap" && SF != "") del = (v in sqset)
		if (del) {
			if (M == "ds_sq_before_delete") { prev = v; havep = 1 }
			return
		}
	} else if (translate) {
		if (v in map) v = map[v]
		else if (M == "drops_unmatched") return
	}
	if (SF != "" && M != "sq_none") {
		insq = (v in sqset) || M == "sq_all"
		if (M == "ds_swap" && DF != "") insq = (v in dset)
		eq = havep && v == prev
		if (M == "sq_before_translate") eq = havep && orig == porig
		if (M == "sq_mixed") eq = havep && (prev in sqset)
		if (insq && eq) {
			if (M == "sq_two" && run < 2) run++
			else { porig = orig; return }
		} else run = 1
	}
	if (M == "d_squeeze_rest" && DF != "" && havep && v == prev) return
	stash(v)
	prev = v; havep = 1; porig = orig
}

BEGIN {
	for (i = 1; i <= 255; i++) ord[sprintf("%c", i)] = i
	M = ENVIRON["M"]; S1 = ENVIRON["S1"]; S2 = ENVIRON["S2"]
	CF = ENVIRON["CF"]; DF = ENVIRON["DF"]; SF = ENVIRON["SF"]
	NOPS = ENVIRON["NOPS"] + 0
	no = 0; havep = 0; run = 1
	n1t = tokenize(S1, 1, c1, k1, m1, r1)
	if (NOPS >= 2) n2t = tokenize(S2, 2, c2, k2, m2, r2)
	conv = 0
	if (NOPS >= 2 && n1t == 1 && n2t == 1 && k1[1] == "c" && k2[1] == "c" && !(DF != "" && SF != "") && \
		((m1[1] == "lower" && m2[1] == "upper") || (m1[1] == "upper" && m2[1] == "lower")))
		conv = 1
	n1 = expand(c1, k1, m1, r1, n1t, A1, 0, conv)
	for (i = 1; i <= n1; i++) in1[A1[i]] = 1
	cplm = 0
	if (CF != "") {
		cplm = 1
		if (M == "c_ignored" && CF == "c") cplm = 0
		if (M == "C_ignored" && CF == "C") cplm = 0
		if (M == "cd_inverse" && DF != "") cplm = 0
	}
	n1o = n1
	for (i = 1; i <= n1; i++) A1o[i] = A1[i]
	if (cplm) {
		top = (M == "c_ascii_only") ? 127 : 255
		n1 = 0
		for (v = 0; v <= top; v++) {
			if (M == "c_excl_newline" && v == 10) continue
			if (!(v in in1)) A1[++n1] = v
		}
	}
	for (i = 1; i <= n1; i++) dset[A1[i]] = 1
	n2 = 0
	if (NOPS >= 2) {
		FILLTOTAL = n1
		base = expand(c2, k2, m2, r2, n2t, A2, 0, conv)
		fill = n1 - base
		if (fill < 0) fill = 0
		n2 = expand(c2, k2, m2, r2, n2t, A2, fill, conv)
		if (conv && M == "l2u_ignored" && m1[1] == "lower") for (i = 1; i <= n1; i++) A2[i] = A1[i]
		if (conv && M == "u2l_ignored" && m1[1] == "upper") for (i = 1; i <= n1; i++) A2[i] = A1[i]
	}
	translate = (DF == "" && NOPS >= 2)
	if (translate) {
		if (M == "swap_operands") {
			for (i = 1; i <= n1 && i <= n2; i++) if (!(A2[i] in map)) map[A2[i]] = A1[i]
		} else {
			for (i = 1; i <= n1; i++) {
				if (M == "map_first_only" && i > 1) break
				if (i <= n2) w = A2[i]
				else if (M == "pad_last" && n2 > 0) w = A2[n2]
				else w = A1[i]
				if (M == "map_off") w = (i < n2) ? A2[i + 1] : A2[1]
				if (M == "dup_last" || !(A1[i] in map)) map[A1[i]] = w
			}
		}
	}
	if (NOPS >= 2) {
		for (i = 1; i <= n2; i++) sqset[A2[i]] = 1
		if (M == "sq_s1" && DF == "") { delete sqset; for (i = 1; i <= n1; i++) sqset[A1[i]] = 1 }
		if (M == "ds_sq_s1" && DF != "") { delete sqset; for (i = 1; i <= n1; i++) sqset[A1[i]] = 1 }
		if (M == "cs_sq_s1" && cplm) { delete sqset; for (i = 1; i <= n1o; i++) sqset[A1o[i]] = 1 }
	} else {
		for (i = 1; i <= n1; i++) sqset[A1[i]] = 1
	}
	if (M == "nonexistent") exit 3
}

{ for (f = 1; f <= NF; f++) proc($f + 0) }

END {
	if (M == "no_final_newline" && no > 0 && out[no - 1] == 10) no--
	for (i = 0; i < no; i++) printf "%c", out[i]
	if (M == "extra_newline") printf "\n"
}

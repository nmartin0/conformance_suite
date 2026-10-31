# A small reimplementation of sort for the POSIX locale (a character is a
# byte) with one planted fault, chosen by the variable M. M empty or unknown:
# behaves correctly. Driven by sortref.sh.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.

function scmp(a, b) { a = a ""; b = b ""; return a < b ? -1 : (a > b ? 1 : 0) }
function isblank_(ch) { return ch == " " || ch == "\t" }
function isdig(ch) { return ch != "" && index("0123456789", ch) > 0 }

# fieldpos(line, f): sets FS_ and FE_ to the first and last character of
# field f (counted from 1); returns 0 when the line has no such field.
function fieldpos(line, f,    n, s, cnt, p, e, i, nf) {
	n = length(line)
	if (T != "") {
		s = 1; cnt = 1
		while (1) {
			p = index(substr(line, s), T)
			e = p ? s + p - 2 : n
			if (cnt == f) {
				FS_ = s; FE_ = e
				if (M == "t_sep_in_field" && s > 1) FS_ = s - 1
				return 1
			}
			if (!p) return 0
			s = s + p
			if (M == "t_collapse") while (substr(line, s, 1) == T) s++
			cnt++
		}
	}
	fst[1] = 1; nf = 1
	for (i = 2; i <= n; i++)
		if (isblank_(substr(line, i, 1)) && !isblank_(substr(line, i - 1, 1))) fst[++nf] = i
	if (f > nf) return 0
	FS_ = fst[f]; FE_ = (f < nf) ? fst[f + 1] - 1 : n
	return 1
}

function extract(line, k,    s, e, es, ee, endp, n) {
	n = length(line)
	if (!fieldpos(line, kSF[k])) return ""
	s = FS_; e = FE_
	if (kSB[k]) while (s <= e && isblank_(substr(line, s, 1))) s++
	s = s + kSC[k] - 1
	if (kEF[k] == 0) endp = n
	else if (!fieldpos(line, kEF[k])) endp = n
	else {
		es = FS_; ee = FE_
		if (kEB[k]) while (es <= ee && isblank_(substr(line, es, 1))) es++
		if (kEC[k] == 0) {
			endp = ee
			if (M == "ec0_empty" && kECg[k]) endp = es - 1
		} else {
			endp = es + kEC[k] - 1
			if (M == "char_end_excl") endp--
			if (endp > ee) endp = ee
		}
	}
	if (s > endp) {
		if (M != "swap_range") return ""
		return substr(line, endp, s - endp + 1)
	}
	return substr(line, s, endp - s + 1)
}

function normalize(s, k,    q) {
	if (kI[k]) {
		if (M == "i_drops_punct") gsub(/[^A-Za-z0-9 \t]/, "", s)
		else gsub(CTRL, "", s)
	}
	if (kD[k]) {
		if (M == "d_blank_dropped") gsub(/[^A-Za-z0-9]/, "", s)
		else if (M == "d_digits_dropped") gsub(/[^A-Za-z \t]/, "", s)
		else gsub(/[^A-Za-z0-9 \t]/, "", s)
	}
	if (kF[k]) s = (M == "f_lower") ? tolower(s) : toupper(s)
	return s
}

# numparse(s): N_neg, N_int, N_frac (digits without leading/trailing
# zeros), N_none (no digit at all)
function numparse(s,    p, n, any) {
	N_neg = 0; N_int = ""; N_frac = ""; N_none = 0
	n = length(s); p = 1
	if (M == "n_no_blanks" && isblank_(substr(s, 1, 1))) return
	while (p <= n && isblank_(substr(s, p, 1))) p++
	if (substr(s, p, 1) == "-") { N_neg = 1; p++ }
	any = 0
	while (p <= n && isdig(substr(s, p, 1))) { N_int = N_int substr(s, p, 1); p++; any = 1 }
	if (substr(s, p, 1) == ".") {
		p++
		while (p <= n && isdig(substr(s, p, 1))) { N_frac = N_frac substr(s, p, 1); p++; any = 1 }
	}
	sub(/^0+/, "", N_int)
	sub(/0+$/, "", N_frac)
	if (M == "n_no_minus") N_neg = 0
	if (M == "n_no_frac") N_frac = ""
	if (N_int == "" && N_frac == "" && M != "n_minus0_less") N_neg = 0
	if (M == "n_text_last" && !any) N_none = 1
}

function numcmp(i, j, k,    c) {
	if (nNone[i, k] || nNone[j, k]) return nNone[i, k] == nNone[j, k] ? 0 : (nNone[i, k] ? 1 : -1)
	if (nNeg[i, k] != nNeg[j, k]) return nNeg[i, k] ? -1 : 1
	c = length(nInt[i, k]) - length(nInt[j, k])
	if (c == 0) c = scmp(nInt[i, k], nInt[j, k])
	if (c == 0) c = scmp(nFrac[i, k], nFrac[j, k])
	if (nNeg[i, k] && M != "n_neg_frac_wrong") c = -c
	return c
}

function keycmp(i, j, lim,    k, c) {
	for (k = 1; k <= lim; k++) {
		if (kN[k]) c = numcmp(i, j, k)
		else c = scmp(kt[i, k], kt[j, k])
		if (kR[k]) c = -c
		if (c != 0) return c
	}
	return 0
}

function fullcmp(i, j,    c) {
	c = keycmp(i, j, nk)
	if (c != 0) return c
	if (UU) return 0
	if (M == "tie_none" || M == "def_first_byte") return 0
	if (M == "n_tie_stable" && G_n) return i < j ? -1 : 1
	c = scmp(L[i], L[j])
	if (M == "f_tie_lower_first" && G_f) return -c
	if (M == "n_tie_forward" && G_n) return c
	if (G_r) c = -c
	return c
}

function ueq(a, b) {
	if (M == "u_whole_line") return (L[a] "") == (L[b] "")
	if (M == "u_first_key") return keycmp(a, b, 1) == 0
	return keycmp(a, b, nk) == 0
}

function msort(lo, hi,    mid, i, j, k) {
	if (lo >= hi) return
	mid = int((lo + hi) / 2)
	msort(lo, mid); msort(mid + 1, hi)
	i = lo; j = mid + 1; k = lo
	while (i <= mid && j <= hi) {
		if (fullcmp(idx[i], idx[j]) <= 0) tmp[k++] = idx[i++]
		else tmp[k++] = idx[j++]
	}
	while (i <= mid) tmp[k++] = idx[i++]
	while (j <= hi) tmp[k++] = idx[j++]
	for (k = lo; k <= hi; k++) idx[k] = tmp[k]
}

function parsepos(s,    p, ch) {
	PF = 0; PC = 0; PCg = 0; Pm = ""
	p = 1
	while (isdig(substr(s, p, 1))) { PF = PF * 10 + substr(s, p, 1); p++ }
	if (substr(s, p, 1) == ".") {
		p++; PCg = 1
		while (isdig(substr(s, p, 1))) { PC = PC * 10 + substr(s, p, 1); p++ }
	}
	Pm = substr(s, p)
}

BEGIN {
	CTRL = "[" sprintf("%c", 1) "-" sprintf("%c", 31) sprintf("%c", 127) "]"
	UU = U
	if (M == "u_ignored") UU = 0
	nkd = (KEYS == "") ? 0 : split(KEYS, kdef, ":")
	if (M == "only_eight_keys" && nkd > 8) nkd = 8
	if (M == "keys_first_only" && nkd > 1) nkd = 1
	if (M == "ninth_rejected" && nkd >= 9) { print "sort: too many keys" > "/dev/stderr"; exit 2 }
	plain = (nkd == 0 && !G_b && !G_d && !G_f && !G_i && !G_n && !G_r)
	nk = nkd ? nkd : 1
	lastmods = ""
	for (q = 1; q <= nkd; q++) {
		src = (M == "keys_reversed") ? nkd - q + 1 : q
		spec = kdef[src]
		c = index(spec, ",")
		sp = c ? substr(spec, 1, c - 1) : spec
		ep = c ? substr(spec, c + 1) : ""
		parsepos(sp)
		kSF[q] = PF; kSC[q] = PC ? PC : 1; sm = Pm
		if (M == "k_field_off1" && kSF[q] > 1) kSF[q]--
		if (M == "char_start_ignored") kSC[q] = 1
		if (M == "char_start_zero" && PCg) kSC[q]++
		if (c) {
			parsepos(ep)
			kEF[q] = PF; kEC[q] = PC; kECg[q] = PCg; em = Pm
		} else {
			kEF[q] = 0; kEC[q] = 0; kECg[q] = 0; em = ""
			if (M == "open_to_field") kEF[q] = kSF[q]
		}
		if (M == "char_end_ignored") kEC[q] = 0
		kmods[q] = sm em
		sbm[q] = sm; ebm[q] = em
	}
	if (nkd == 0) {
		kSF[1] = 1; kSC[1] = 1; kEF[1] = 0; kEC[1] = 0; kECg[1] = 0
		kmods[1] = ""; sbm[1] = ""; ebm[1] = ""
	}
	if (M == "mod_global_last" && nkd > 1 && kmods[nkd] ~ /[dfinr]/)
		for (q = 1; q < nkd; q++) kmods[q] = kmods[nkd]
	for (q = 1; q <= nk; q++) {
		mods = kmods[q]
		if (M == "mod_ignored") { gsub(/[dfinr]/, "", mods) }
		if (mods ~ /[dfinr]/) {
			kD[q] = index(mods, "d") > 0; kF[q] = index(mods, "f") > 0
			kI[q] = index(mods, "i") > 0; kN[q] = index(mods, "n") > 0; kR[q] = index(mods, "r") > 0
			if (M == "key_mod_adds_global") {
				kD[q] = kD[q] || G_d; kF[q] = kF[q] || G_f; kI[q] = kI[q] || G_i; kN[q] = kN[q] || G_n; kR[q] = kR[q] || G_r
			}
		} else if (M == "global_not_applied" && nkd > 0) {
			kD[q] = kF[q] = kI[q] = kN[q] = kR[q] = 0
		} else if (M == "global_first_key_only" && q > 1) {
			kD[q] = kF[q] = kI[q] = kN[q] = kR[q] = 0
		} else {
			kD[q] = G_d; kF[q] = G_f; kI[q] = G_i; kN[q] = G_n; kR[q] = G_r
		}
		if (M == "global_r_last_key_only" && q < nk && mods !~ /[dfinr]/) kR[q] = 0
		gb = G_b
		if (M == "b_first_key_only" && q > 1) gb = 0
		kSB[q] = (index(sbm[q], "b") > 0 && M != "keyb_ignored") || gb
		kEB[q] = (index(ebm[q], "b") > 0 && M != "keyb_ignored" && M != "b_end_ignored") || (gb && M != "b_end_ignored")
		if (M == "b_ignored") kSB[q] = kEB[q] = 0
	}
	N = 0
}

{ L[++N] = $0 }

END {
	for (i = 1; i <= N; i++)
		for (k = 1; k <= nk; k++) {
			raw = extract(L[i], k)
			if (plain && M == "def_first_byte") raw = substr(raw, 1, 1)
			if (plain && M == "def_length_first") raw = sprintf("%08d", length(raw)) raw
			if (kN[k]) {
				numparse(raw)
				nNeg[i, k] = N_neg; nInt[i, k] = N_int; nFrac[i, k] = N_frac; nNone[i, k] = N_none
			} else kt[i, k] = normalize(raw, k)
		}
	if (plain && M == "def_fold") for (i = 1; i <= N; i++) kt[i, 1] = toupper(kt[i, 1])
	if (plain && M == "def_numeric") {
		kN[1] = 1
		for (i = 1; i <= N; i++) { numparse(L[i]); nNeg[i, 1] = N_neg; nInt[i, 1] = N_int; nFrac[i, 1] = N_frac; nNone[i, 1] = N_none }
	}
	if (plain && M == "def_reverse") kR[1] = 1
	xst = 0
	if (M == "status1_always" || (M == "r_status1" && G_r) || (M == "u_status1" && U) || (M == "m_status1" && MG)) xst = 1
	if (C) {
		bad = 0
		lo = 2; hi = N
		if (M == "c_skips_first") lo = 3
		if (M == "c_misses_last") hi = N - 1
		for (i = lo; i <= hi && !bad; i++) {
			if (UU && M != "u_dup_ok" && keycmp(i - 1, i, nk) == 0) bad = i
			else if (M == "c_ok_status1") continue
			else if (fullcmp(i - 1, i) > 0) bad = i
		}
		if (bad) {
			if ((C == 1 && M != "c_no_diag") || (C == 2 && M == "C_diag")) print "sort: disorder: " L[bad] > "/dev/stderr"
			if (M == "c_prints") for (i = 1; i <= N; i++) print L[i]
			if (M == "c_status0") exit 0
			if (M == "c_status2") exit 2
			exit 1
		}
		if (M == "c_ok_status1") exit 1
		exit xst
	}
	for (i = 1; i <= N; i++) idx[i] = i
	if (M == "r_input_order" && G_r) { for (i = N; i >= 1; i--) idx[N - i + 1] = i }
	else if (!(MG && M == "m_concat")) msort(1, N)
	nr = 0
	for (p = 1; p <= N; p++) {
		i = idx[p]
		if (UU && nr > 0 && ueq(R[nr], i)) { dup[nr] = 1; if (M == "u_last") R[nr] = i; continue }
		R[++nr] = i
	}
	if (M == "m_dedupe" && MG) {
		for (p = nr; p > 1; p--) if ((L[R[p]] "") == (L[R[p - 1]] "")) R[p] = 0
	}
	if (M == "u_drop_all") for (p = 1; p <= nr; p++) if (dup[p]) R[p] = 0
	last = 0
	for (p = 1; p <= nr; p++) if (R[p]) last = p
	for (p = 1; p <= nr; p++) {
		if (!R[p]) continue
		if (M == "no_final_nl" && p == last) printf "%s", L[R[p]]
		else print L[R[p]]
	}
	exit xst
}

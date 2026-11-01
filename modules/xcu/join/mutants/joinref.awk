# Reimplementation of join for the POSIX locale (a character is a byte,
# comparison is by byte value) with one planted fault, chosen by M.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.

function lowercase(s) { return tolower(s) }

# split_fields(line, arr): fields of a line; returns their number.
function split_fields(line, arr,    n, i, c, cur, p, s) {
	n = 0
	if (TSET) {
		if (M == "t_ignored") return split_default(line, arr)
		cur = ""
		for (i = 1; i <= length(line); i++) {
			c = substr(line, i, 1)
			if (c == SEP || (M == "t_blank_split" && (c == " " || c == "\t"))) {
				if (M == "t_collapse" && cur == "" && n > 0 && substr(line, i - 1, 1) == c) continue
				arr[++n] = cur
				cur = ""
			} else cur = cur c
		}
		arr[++n] = cur
		if (M == "t_leading_dropped" && substr(line, 1, 1) == SEP) {
			for (i = 1; i < n; i++) arr[i] = arr[i + 1]
			n--
		}
		if (M == "t_blank_strip") for (i = 1; i <= n; i++) sub(/^[ \t]+/, "", arr[i])
		return n
	}
	return split_default(line, arr)
}

function split_default(line, arr,    n, i, c, cur, re) {
	re = (M == "space_only") ? "[ ]+" : "[ \t]+"
	if (M == "blank_each") {
		sub(/^[ \t]+/, "", line)
		n = 0; cur = ""
		for (i = 1; i <= length(line); i++) {
			c = substr(line, i, 1)
			if (c == " " || c == "\t") { arr[++n] = cur; cur = "" } else cur = cur c
		}
		if (cur != "" || n > 0) arr[++n] = cur
		return n
	}
	if (M != "lead_blank_field") {
		if (M == "space_only") sub(/^ +/, "", line); else sub(/^[ \t]+/, "", line)
	}
	if (M == "space_only") sub(/ +$/, "", line); else sub(/[ \t]+$/, "", line)
	if (line == "") return 0
	return split(line, arr, re)
}

function keyof(line, k, wf,    arr, n, s) {
	n = split_fields(line, arr)
	if (M == "zero_based") k = k + 1
	if (M == "single_digit") k = substr(k, 1, 1) + 0
	s = (k <= n) ? arr[k] : ""
	if (M == "ignore_case") s = tolower(s)
	if (M == "prefix_key") s = substr(s, 1, 1)
	if (M == "t_trim_key" && TSET) sub(/^[ \t]+/, "", s)
	return s
}

function add(s) { OUT[++ON] = s }

# defaults: the other fields of one line, with the join field index k
function others(line, k, which,    arr, n, i, pre, post, np, nq, a) {
	n = split_fields(line, arr)
	if (M == "zero_based") k = k + 1
	if (M == "single_digit") k = substr(k, 1, 1) + 0
	np = 0; nq = 0
	for (i = 1; i <= n; i++) {
		if (i < k) PRE[++np] = arr[i]
		else if (i > k) POST[++nq] = arr[i]
	}
	if (M == "follow_first" || (M == "follow_first_2" && which == 2)) {
		for (i = 1; i <= nq; i++) add(POST[i])
		for (i = 1; i <= np; i++) add(PRE[i])
	} else {
		for (i = 1; i <= np; i++) add(PRE[i])
		for (i = 1; i <= nq; i++) add(POST[i])
	}
}

function allfields(line,    arr, n, i) {
	n = split_fields(line, arr)
	for (i = 1; i <= n; i++) add(arr[i])
}

# one -o item for the lines l1 and l2 (either may be absent: has1/has2)
function item(it, l1, has1, l2, has2, key,    p, f, fl, line, has, arr, n, v, absent) {
	absent = 0
	if (it == "0") {
		if (M == "o_zero_unpaired_empty" && !(has1 && has2)) return ""
		if (M == "o_zero_missing") return ""
		if (M == "o_zero_f1") { n = split_fields(l1, arr); return (1 <= n) ? arr[1] : "" }
		return key
	}
	p = index(it, ".")
	fl = substr(it, 1, p - 1) + 0
	f = substr(it, p + 1) + 0
	if (M == "o_single_digit") f = substr(substr(it, p + 1), 1, 1) + 0
	if (M == "o_off_by_one") f = f + 1
	if (M == "o_swap_files") fl = 3 - fl
	if (fl == 1) { line = l1; has = has1 } else { line = l2; has = has2 }
	if (!has) { LASTABSENT = 1; return "" }
	n = split_fields(line, arr)
	if (f > n) { LASTABSENT = 1; return "" }
	LASTABSENT = 0
	return arr[f]
}

function replace(v, absent, fl, last) {
	if (!ESET || M == "e_ignored") return v
	if (v != "") { if (M == "e_all") return E; return v }
	if (M == "e_not_empty" && !absent) return v
	if (M == "e_not_absent" && absent) return v
	if (M == "e_f1_only" && fl == 2) return v
	if (M == "e_last_skipped" && last) return v
	if (M == "e_first_char") return substr(E, 1, 1)
	if (M == "e_word") { sub(/[ \t].*/, "", E2); return E2 }
	return E
}

# parse the -o list into IT[1..NI]
function parse_list(   s, n, i, parts, j, t, u) {
	s = OL
	if (M == "o_comma_only") n = split(s, parts, ",")
	else if (M == "o_space_only") { gsub(/,/, " ", s); n = split(s, parts, " ") }
	else { gsub(/,/, " ", s); gsub(/\t/, " ", s); n = split(s, parts, " ") }
	NI = 0
	for (i = 1; i <= n; i++) {
		if (parts[i] == "") continue
		if (M == "o_dup_dropped") {
			t = 0
			for (j = 1; j <= NI; j++) if (IT[j] == parts[i]) t = 1
			if (t) continue
		}
		IT[++NI] = parts[i]
	}
	if (M == "o_sorted") {
		for (i = 1; i <= NI; i++) for (j = i + 1; j <= NI; j++) {
			t = IT[i]; u = IT[j]
			if (ordkey(u) < ordkey(t)) { IT[i] = u; IT[j] = t }
		}
	}
}
function ordkey(x,    p) {
	if (x == "0") return 0
	p = index(x, ".")
	return substr(x, 1, p - 1) * 1000 + substr(x, p + 1)
}

# emit one output line from the lines l1/l2 (has1/has2 say which exist)
function emit_pair(l1, has1, l2, has2, key, kind,    i, v, fl, absent, itm) {
	ON = 0
	if (OLSET && M != "o_ignored" && !(M == "o_unpaired_default" && kind != "pair")) {
		if (M == "o_with_key") add(key)
		for (i = 1; i <= NI; i++) {
			v = item(IT[i], l1, has1, l2, has2, key)
			absent = LASTABSENT
			if (IT[i] == "0") absent = 0
			fl = (IT[i] == "0") ? 0 : substr(IT[i], 1, 1) + 0
			add(replace(v, absent, fl, i == NI))
		}
	} else if (kind == "pair") {
		if (M != "drop_key") add(key)
		if (M == "key_stays") {
			ON = 0
			allfields(l1)
			others(l2, K2, 2)
		} else if (M == "swap_files") {
			others(l2, K2, 2); others(l1, K1, 1)
		} else {
			others(l1, K1, 1); others(l2, K2, 2)
		}
	} else if (kind == "u1") {
		if ((M == "a_raw_line" && AFLAG) || (M == "v_raw_line" && VFLAG)) allfields(l1)
		else { if (!((M == "a_drop_key" && AFLAG))) add(key); others(l1, K1, 1) }
	} else {
		if ((M == "a_raw_line" && AFLAG) || (M == "v_raw_line" && VFLAG)) allfields(l2)
		else { if (!((M == "a_drop_key" && AFLAG))) add(key); others(l2, K2, 2) }
	}
	s = ""
	for (i = 1; i <= ON; i++) s = s (i > 1 ? OS : "") OUT[i]
	LINES[++NLINES] = s
}

function emit_unpaired(side, line, key,    q) {
	if (side == 1) emit_pair(line, 1, "", 0, key, "u1"); else emit_pair("", 0, line, 1, key, "u2")
	if (M == "a_twice" && AFLAG) { NLINES++; LINES[NLINES] = LINES[NLINES - 1] }
}

function want_unpaired(side,    s) {
	s = side
	if (M == "a_swapped" || M == "v_swapped") s = 3 - side
	if (M == "pair_unmatched") return 1
	if (s == 1) return (A1 && M != "a1_ignored") || (V1 && M != "v1_ignored")
	return (A2 && M != "a2_ignored") || (V2 && M != "v2_ignored")
}

function readfile(f, L,    n, line) {
	n = 0
	while ((getline line < f) > 0) {
		n++
		if (M == "cap_input" && n > 1024) break
		L[n] = line
	}
	close(f)
	return n
}

BEGIN {
	E2 = E
	AFLAG = (A1 || A2); VFLAG = (V1 || V2)
	OLSET = (OL != "")
	OS = " "
	if (TSET) { OS = SEP; if (M == "out_space") OS = " " }
	else if (M == "sep_tab") OS = "\t"
	if (OLSET && M == "o_sep_space") OS = " "
	if (OLSET) parse_list()
	n1 = readfile(F1, L1); n2 = readfile(F2, L2)
	DEFER = (M == "a_at_end" && AFLAG) || (M == "v_at_end" && VFLAG)
	NDEF = 0
	if (M == "a_empty_nothing" && (n1 == 0 || n2 == 0)) exit 0
	showpairs = !(V1 || V2) || M == "v_with_pairs"
	if (M == "a_as_v" && AFLAG) showpairs = 0
	i = 1; j = 1
	while (i <= n1 || j <= n2) {
		if (i <= n1) k1 = keyof(L1[i], K1, 1)
		if (j <= n2) k2 = keyof(L2[j], K2, 2)
		if (j > n2 || (i <= n1 && k1 < k2)) {
			if (want_unpaired(1)) {
				if (DEFER) { before = NLINES; emit_unpaired(1, L1[i], k1); mv(before) } else emit_unpaired(1, L1[i], k1)
			}
			i++
		} else if (i > n1 || k2 < k1) {
			if (want_unpaired(2)) {
				if (DEFER) { before = NLINES; emit_unpaired(2, L2[j], k2); mv(before) } else emit_unpaired(2, L2[j], k2)
			}
			j++
		} else {
			ie = i; while (ie + 1 <= n1 && keyof(L1[ie + 1], K1, 1) == k1) ie++
			je = j; while (je + 1 <= n2 && keyof(L2[je + 1], K2, 2) == k2) je++
			if (showpairs) groups(i, ie, j, je, k1)
			i = ie + 1; j = je + 1
		}
	}
	for (d = 1; d <= NDEF; d++) LINES[++NLINES] = DEFL[d]
	out = NLINES
	if (M == "drop_last") out--
	if (M == "stop_after_first" && out > 1) out = 1
	for (d = 1; d <= out; d++) {
		s = LINES[d]
		if (M == "trunc_line") s = substr(s, 1, 512)
		if (M == "no_final_newline" && d == out) printf "%s", s
		else printf "%s\n", s
	}
	if (M == "v_empty_status1" && NLINES == 0) exit 1
	exit 0
}

# move lines emitted since "before" to the deferred list
function mv(before,    d) {
	for (d = before + 1; d <= NLINES; d++) DEFL[++NDEF] = LINES[d]
	NLINES = before
}

function groups(i1, i2, j1, j2, key,    a, b, c, n, m) {
	if (M == "rep_zip") {
		n = i2 - i1 + 1; m = j2 - j1 + 1
		if (m < n) n = m
		for (a = 0; a < n; a++) emit_pair(L1[i1 + a], 1, L2[j1 + a], 1, key, "pair")
		return
	}
	if (M == "rep_swapped_order") {
		for (b = j1; b <= j2; b++) for (a = i1; a <= i2; a++) emit_pair(L1[a], 1, L2[b], 1, key, "pair")
		return
	}
	if (M == "rep_reverse") {
		for (a = i2; a >= i1; a--) for (b = j2; b >= j1; b--) emit_pair(L1[a], 1, L2[b], 1, key, "pair")
		return
	}
	if (M == "rep_f2_first_only") j2 = j1
	if (M == "rep_f2_last_only") j1 = j2
	if (M == "rep_f1_first_only") i2 = i1
	if (M == "rep_f1_last_only") i1 = i2
	for (a = i1; a <= i2; a++) for (b = j1; b <= j2; b++) emit_pair(L1[a], 1, L2[b], 1, key, "pair")
}

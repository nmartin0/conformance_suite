# Reference uniq with one planted fault M (see uniqref.sh).
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
function skipfields(s, n,   i) {
	for (i = 0; i < n; i++) {
		if (M == "f_trailing") match(s, /^[^ \t]*[ \t]*/)
		else if (M == "f_spaceonly") match(s, /^ *[^ ]*/)
		else if (M == "f_single") match(s, /^[ \t]?[^ \t]*/)
		else match(s, /^[ \t]*[^ \t]*/)
		s = substr(s, RLENGTH + 1)
	}
	return s
}
function skipchars(s, n,   t, i, c) {
	if (M == "s_nonblank") {
		t = ""
		for (i = 1; i <= length(s); i++) {
			c = substr(s, i, 1)
			if (n > 0 && c != " " && c != "\t") { n--; continue }
			if (n > 0) continue
			t = t c
		}
		return t
	}
	return substr(s, n + 1)
}
function nfields(u,   n) {
	n = 0
	while (match(u, /^[ \t]*[^ \t]+/)) { n++; u = substr(u, RLENGTH + 1) }
	return n
}
function key(s,   f, c) {
	f = nf; c = nc
	if (M == "f_ignored") f = 0
	if (M == "f_plus1") f++
	if (M == "f_minus1" && f > 0) f--
	if (M == "s_ignored") c = 0
	if (M == "s_plus1") c++
	if (M == "s_minus1" && c > 0) c--
	if (M == "s_ignored_with_f" && f > 0) c = 0
	if (M == "f_ignored_with_s" && c > 0) f = 0
	if (M == "f_chars") { s = substr(s, f + 1); f = 0 }
	if (M == "short_f_whole" && f > 0 && nfields(s) < f) return s
	if (M == "short_s_whole" && c > length(s)) return s
	if (M == "s_ge_whole" && c >= length(s) && c > 0 && f == 0) return s
	if (M == "s_before_f") { s = skipchars(s, c); s = skipfields(s, f) }
	else if (M == "f_s_as_fields") { s = skipfields(s, f + c) }
	else { s = skipfields(s, f); s = skipchars(s, c) }
	if (M == "ignore_case") s = tolower(s)
	if (M == "first_char") s = substr(s, 1, 1)
	if (M == "trunc1024") s = substr(s, 1, 1024)
	if (M == "trailing_blank_ignored") sub(/[ \t]+$/, "", s)
	return s
}
BEGIN {
	if (M == "short_f_error" && nf > 0 || M == "short_s_error" && nc > 0) shortcheck = 1
}
{
	k = key($0)
	if (shortcheck && length($0) < nc && M == "short_s_error") err = 1
	if (shortcheck && M == "short_f_error") {
		if (nfields($0) < nf) err = 1
	}
	if (M == "drops_empty" && $0 == "") next
	total[k]++
	if (ng > 0 && k == gk[ng] && !(M == "never_collapse")) {
		gc[ng]++
		gl[ng] = $0
	} else if (M == "nonadj" && (k in idx)) {
		gc[idx[k]]++
		gl[idx[k]] = $0
	} else {
		ng++
		gk[ng] = k; gc[ng] = 1; gf[ng] = $0; gl[ng] = $0
		idx[k] = ng
	}
}
END {
	if (err) { print "uniq: line too short" > "/dev/stderr"; exit 1 }
	np = 0
	for (g = 1; g <= ng; g++) {
		c = gc[g]; t = total[gk[g]]
		line = gf[g]
		if (M == "keep_last") line = gl[g]
		n = 1
		if (mode == "d") {
			if (M == "d_all") ok = 1
			else if (M == "d_inverted") ok = (c == 1)
			else if (M == "d_ge3") ok = (c >= 3)
			else if (M == "d_exactly2") ok = (c == 2)
			else if (M == "nonadj_d") ok = (t >= 2)
			else ok = (c >= 2)
			if (M == "d_every_copy") n = c
		} else if (mode == "u") {
			if (M == "u_all") ok = 1
			else if (M == "u_inverted") ok = (c >= 2)
			else if (M == "u_ge3") ok = (c < 3)
			else if (M == "nonadj_u") ok = (t == 1)
			else ok = (c == 1)
		} else ok = 1
		if (M == "c_drop_single" && mode == "c" && c == 1) ok = 0
		if (!ok) continue
		np++
		for (i = 0; i < n; i++) {
			if (mode == "c") {
				v = c
				if (M == "c_zero") v = c - 1
				if (M == "c_one") v = 1
				if (M == "c_mod10") v = c % 10
				if (M == "c_global") v = t
				if (M == "c_after") out[np] = line " " v
				else if (M == "c_nosep") out[np] = v line
				else if (M == "c_tab") out[np] = v "\t" line
				else if (M == "c_only_count") out[np] = v
				else out[np] = v " " line
			} else out[np] = line
			if (n > 1 && i < n - 1) np++
		}
	}
	for (i = 1; i <= np; i++) {
		if (M == "d_first_only" && mode == "d" && i > 1) break
		if (M == "u_drops_last" && mode == "u" && i == np) break
		if (M == "u_drops_first" && mode == "u" && i == 1) continue
		if (M == "no_newline" && i == np) printf "%s", out[i]
		else print out[i]
	}
	if (M == "status1_end") exit 1
}

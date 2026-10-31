# Reference cut with one planted fault M (see cutref.sh).
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
function pick(n,   i, k, c, j, u) {
	np = 0
	if (M == "list_order") {
		for (k = 1; k <= ne; k++)
			for (i = lo[k]; i <= hi[k] && i <= n; i++)
				if (!(i in u)) { u[i] = 1; ord[++np] = i }
		return
	}
	for (i = 1; i <= n; i++) {
		c = 0
		for (k = 1; k <= ne; k++)
			if (lo[k] <= i && i <= hi[k]) c++
		if (c > 0) {
			if (M == "dup") for (j = 0; j < c; j++) ord[++np] = i
			else ord[++np] = i
		}
	}
}
function emit(s) {
	if (M == "no_newline") printf "%s", s
	else print s
}
BEGIN {
	INF = 1000000000
	if (M == "blank_stops") sub(/ .*/, "", list)
	if (M == "blank_rejects" && index(list, " ")) {
		print "cut: bad list" > "/dev/stderr"
		bad = 1; exit 1
	}
	gsub(/ /, ",", list)
	ne = split(list, el, ",")
	if (M == "list_first") ne = 1
	for (k = 1; k <= ne; k++) {
		e = el[k]
		if (e ~ /^[0-9]+$/) {
			lo[k] = hi[k] = e + 0
			if (M == "single_off") lo[k] = hi[k] = e + 1
		} else if (e ~ /^-[0-9]+$/) {
			lo[k] = 1; hi[k] = substr(e, 2) + 0
			if (M == "head_excl") hi[k]--
		} else if (e ~ /^[0-9]+-$/) {
			lo[k] = substr(e, 1, length(e) - 1) + 0; hi[k] = INF
			if (M == "open_off") lo[k]++
		} else {
			split(e, p, "-")
			lo[k] = p[1] + 0; hi[k] = p[2] + 0
			if (M == "range_excl") hi[k]--
			if (M == "range_swap_ok_low") lo[k]++
		}
		if (M == "n_drop" && nflag && hi[k] < INF) hi[k]--
	}
	if (M == "n_reject" && nflag) {
		print "cut: -n not supported" > "/dev/stderr"
		bad = 1; exit 1
	}
	rc = 0
}
mode == "f" {
	if (index($0, delim) == 0) {
		if (M == "suppress_inverse") { if (sflag) { emit($0) } ; next }
		if (M == "suppress_always") next
		if (sflag && M != "suppress_ignored") next
		emit($0)
		next
	}
	if (M == "suppress_inverse" && sflag) next
	nf = 0; rest = $0
	while ((i = index(rest, delim)) > 0) {
		f[++nf] = substr(rest, 1, i - 1)
		rest = substr(rest, i + length(delim))
	}
	f[++nf] = rest
	pick(nf)
	sep = delim
	if (M == "outsep_tab") sep = "\t"
	if (M == "outsep_space") sep = " "
	out = ""
	for (j = 1; j <= np; j++) out = out (j > 1 ? sep : "") f[ord[j]]
	miss = 0
	for (k = 1; k <= ne; k++) if (lo[k] > nf) miss++
	if (miss && M == "absent_error") rc = 1
	if (np == 0 && M == "absent_skipline") next
	if (M == "absent_trailing") for (k = 1; k <= miss; k++) out = out sep
	emit(out)
	next
}
{
	n = length($0)
	if (M == "all_ignored") { emit($0); next }
	pick(n)
	out = ""
	for (j = 1; j <= np; j++) out = out substr($0, ord[j], 1)
	if (M == "beyond_pad") {
		mx = 0
		for (k = 1; k <= ne; k++) if (hi[k] < INF && hi[k] > mx) mx = hi[k]
		for (j = n; j < mx; j++) out = out " "
	}
	if (M == "beyond_error")
		for (k = 1; k <= ne; k++) if (lo[k] > n || (hi[k] < INF && hi[k] > n)) rc = 1
	if (M == "beyond_skipempty" && out == "") next
	emit(out)
}
END {
	if (bad) exit 1
	if (M == "status1") exit 1
	exit rc
}

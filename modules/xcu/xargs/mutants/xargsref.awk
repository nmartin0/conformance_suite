# Shared by the xargs faults and variants: reads the decimal byte values of
# the standard input (one or more per line) and writes the batches of
# arguments, each as a file of single-quoted words, into directory dir.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# M names the planted fault (empty for a correct run). A failure to assemble
# a command line is recorded in the file dir/fail.
function sq(t,   r, i, c) {
	r = "'"
	for (i = 1; i <= length(t); i++) {
		c = substr(t, i, 1)
		if (c == "'") r = r "'\\''"
		else r = r c
	}
	return r "'"
}
function fail(msg) {
	failed = 1
	print msg > (dir "/fail")
	close(dir "/fail")
	stop = 1
}
function put(text) {
	nb++
	f = sprintf("%s/b.%06d", dir, nb)
	printf "%s", text > f
	close(f)
}
function flushb() {
	if (M == "r_never" && rflag) { bc = 0; bsz = 0; bq = ""; return }
	put(bq)
	bc = 0; bsz = 0; bq = ""
}
function add(t,   len, lim) {
	if (mode == "I") {
		if (M == "i_batches") { bq = bq (bq == "" ? "" : " ") sq(t); bc++; return }
		put(sq(t))
		return
	}
	len = length(t) + (M == "s_no_nul" ? 0 : 1)
	lim = s
	if (!(bc > 0 && n > 0 && bc >= n) && !(M == "x_runs_anyway" && xflag && n > 0)) {
		if (base + bsz + len >= lim + (M == "s_le" ? 1 : 0)) {
			if (bc > 0 && xflag && n > 0 && M != "x_ignored") { fail("xargs: argument list too long"); return }
			if (M == "nox_fail" && !xflag && bc > 0) { fail("xargs: argument list too long"); return }
			if (bc > 0) {
				if (M == "nox_drops" && !xflag) return
				flushb()
			}
			if (base + len >= lim + (M == "s_le" ? 1 : 0)) { fail("xargs: argument too long"); return }
		}
	}
	bq = bq (bc ? " " : "") sq(t)
	bc++; bsz += len
	lastfull = 0
	if (n > 0 && bc >= n) { flushb(); lastfull = 1 }
}
function end_token(   t) {
	t = cur; cur = ""; intok = 0
	if (mode == "I" && M == "i_trail_trim") {
		while (substr(t, length(t), 1) == " ") t = substr(t, 1, length(t) - 1)
	}
	if (mode == "I" && M == "i_cap100") t = substr(t, 1, 100)
	if (eofline) { quoted = 0; return }
	if (heof && eof != "" && M != "eof_ignored") {
		hit = (t == eof)
		if (M == "eof_substring" && index(t, eof) == 1) hit = 1
		if (M == "eof_before_quote" && quoted) hit = 0
		if (hit) {
			quoted = 0
			if (M == "eof_inclusive") { add(t); stop = 1; return }
			if (M == "eof_after") return
			if (M == "eof_nextline") { eofline = 1; return }
			stop = 1
			return
		}
	}
	if (M == "eof_empty_underscore" && heof && eof == "" && t == "_") { stop = 1; return }
	quoted = 0
	add(t)
	linehas = 1
	if (mode == "L" && M == "l_words") { lines++; if (lines >= Lc) { lines = 0; flushb() } }
}
function newline() {
	if (mode != "L") return
	if (linehas) {
		if (lastblank && M != "l_no_continue") { lastblank = 0; return }
		linehas = 0
		lines++
	} else if (M == "l_empty_counts") {
		lines++
	} else return
	if (M != "l_words" && Lc > 0 && lines >= Lc) { lines = 0; if (bc > 0) flushb() }
}
function process(b,   c) {
	if (stop) return
	c = chr[b]
	if (zero) {
		if (b == 0) {
			if (M == "zero_collapse" && !intok) return
			inq = 0
			end_token(); return
		}
		if (M == "zero_ws" && (b == 32 || b == 9 || b == 10)) { if (intok) end_token(); return }
		if (M == "zero_nl" && b == 10) { if (intok) end_token(); return }
		if (M == "zero_quotes" && !inq && (b == 34 || b == 39)) { inq = b; intok = 1; return }
		if (M == "zero_quotes" && inq && b == inq) { inq = 0; return }
		cur = cur c; intok = 1
		return
	}
	if (esc) {
		esc = 0
		cur = cur (M == "bs_keep" ? "\\" : "") c; intok = 1; lastblank = 0; quoted = 1
		return
	}
	if (inq) {
		if (b == inq) inq = 0
		else if (M == "dq_ends_at_sq" && inq == 34 && b == 39) { }
		else cur = cur c
		return
	}
	if (b == 92 && M != "bs_ignored") { esc = 1; intok = 1; quoted = 1; lastblank = 0; return }
	if (b == 34 && M != "dq_ignored") { inq = 34; intok = 1; quoted = 1; lastblank = 0; return }
	if (b == 39 && M != "sq_ignored") { inq = 39; intok = 1; quoted = 1; lastblank = 0; return }
	if (b == 32 || b == 9) {
		if (mode == "I" && M != "i_words") {
			if (intok || M == "i_no_trim") { cur = cur c; intok = 1 }
			return
		}
		if (M == "sep_nl_only") { cur = cur c; intok = 1; lastblank = 0; return }
		if (intok) end_token()
		else if (M == "empty_arg_on_run" && wasblank && !stop) add("")
		lastblank = linehas ? 1 : 0
		wasblank = 1
		return
	}
	if (b == 10) {
		if (M == "sep_blank_only") { cur = cur c; intok = 1; return }
		if (intok) end_token()
		eofline = 0
		newline()
		lastblank = 0
		wasblank = 0
		return
	}
	cur = cur c; intok = 1; lastblank = 0; wasblank = 0
}
BEGIN {
	for (i = 1; i < 256; i++) chr[i] = sprintf("%c", i)
	chr[0] = ""
	heof = hasE
	Lc = (M == "l_plus1" ? L + 1 : L)
	if (M == "l_ignored") Lc = 0
	if (mode == "L" && L == 0) Lc = 0
	if (M == "n_plus1") n = n + 1
	if (M == "n_minus1" && n > 1) n = n - 1
	if (M == "n_ignored") n = 0
	if (M == "s_ignored") s = 1000000000
	if (M == "s_no_nul") base = base - nstr
	if (M == "s_no_name") base = base - ulen - 1
	if (M == "eof_empty_underscore" && hasE && eof == "") heof = 1
	if (M == "r_ignored") rflag = 0
}
{
	for (i = 1; i <= NF; i++) { anyb = 1; process($i + 0) }
}
END {
	if (!failed) {
		if (zero && intok && M != "zero_trailing_used") {
			cur = ""; intok = 0
			if (M == "zero_trailing_garbage") add("JUNK")
		}
		if (!stop && (intok || inq)) end_token()
		if (bc > 0) {
			if (M == "n_last_dropped" && n > 0) { }
			else if (M == "l_last_dropped" && mode == "L") { }
			else if (M == "x_last_error" && xflag && n > 0 && bc < n) fail("xargs: argument list too long")
			else flushb()
		}
	}
	if (!failed && nb == 0 && mode != "I") {
		sup = rflag && !(M == "r_blank_runs" && anyb)
		if (!sup && M != "noinput_norun") {
			put("")
			if (M == "runs_twice") put("")
		}
	}
	if (!failed && M == "n_extra_run" && lastfull && mode == "def") put("")
}

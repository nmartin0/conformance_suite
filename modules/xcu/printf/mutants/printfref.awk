# A small reimplementation of the printf utility for the POSIX locale (a
# character is a byte) with one planted fault or conforming variant,
# chosen by the variable M. The format and the arguments arrive as
# ARGV[1..], each with one marker byte in front (so that awk takes none of
# them for an assignment or a file).
# Copyright (c) 2026 Nicholas Martin. All rights reserved.

# the radix character of the locale in effect for LC_NUMERIC (LOC is worked
# out by printfrun.sh); only the variant that models a system with floating
# point and made locales uses it
function radix() { return (LOC == "C" || LOC == "POSIX") ? "." : "," }

function emit(s) {
	if (s == "") return
	if (M == "lit_firstlost" && !dropped) {
		dropped = 1
		s = substr(s, 2)
	}
	printf "%s", s
}

function diag(msg) {
	if (M == "bad_no_diag") return
	if (M == "bad_diag_stdout") printf "printf: %s\n", msg
	else print "printf: " msg > "/dev/stderr"
}

function ord(c,   i) {
	for (i = 1; i < 256; i++) if (chr[i] == c) return i
	return 0
}

function isodig(c) { return c != "" && index("01234567", c) > 0 }
function isdig(c) { return c != "" && index("0123456789", c) > 0 }
function ishex(c) { return c != "" && index("0123456789abcdefABCDEF", c) > 0 }
function hexval(c) { return index("0123456789abcdef", tolower(c)) - 1 }

# the number an argument stands for; badnum is set when it is not entirely one
function tonum(s,   c, v, i, n, sign, rest, base, any) {
	badnum = 0
	if (s == "") return 0
	c = substr(s, 1, 1)
	if (c == "'" || c == "\"") {
		if (M == "d_quote_zero") return 0
		if (M == "d_quote_is_quote") return ord(c)
		if (M == "d_dquote_unsupported" && c == "\"") { badnum = 1; return 0 }
		return ord(substr(s, 2, 1))
	}
	sign = 1
	i = 1
	if (c == "+") {
		i = 2
		if (M == "d_plus_error") { badnum = 1; return 0 }
	} else if (c == "-") {
		i = 2
		sign = (M == "d_minus_dropped") ? 1 : -1
	}
	rest = substr(s, i)
	v = 0
	if (rest ~ /^0[xX]/ && M != "d_hex_unsupported" && !(M == "d_hexX_unsupported" && rest ~ /^0X/)) {
		n = 3
		any = 0
		while (ishex(substr(rest, n, 1))) {
			v = v * 16 + hexval(substr(rest, n, 1))
			n++
			any = 1
		}
		if (n <= length(rest) || !any) badnum = 1
		return sign * v
	}
	if (rest ~ /^0/ && M != "d_octal_decimal") {
		n = 1
		while (isodig(substr(rest, n, 1))) {
			v = v * 8 + substr(rest, n, 1)
			n++
		}
		if (n <= length(rest)) badnum = 1
		return sign * v
	}
	n = 1
	while (isdig(substr(rest, n, 1))) {
		v = v * 10 + substr(rest, n, 1)
		n++
	}
	if (n <= length(rest) || n == 1) badnum = 1
	if (badnum && M == "bad_partial_dropped") v = 0
	return sign * v
}

# the next argument for a conversion: sets have (0 when it is missing)
function getarg(num,   idx) {
	if (num != "") {
		idx = (M == "n_zero_based") ? base + num + 1 : base + num
		if (M == "n_reuse_base0") idx = num
		if (M == "n_reverse") idx = nargs - num + 1
		if (M == "n_ignored") { idx = pos; pos++ }
		if (num + 0 > maxn) maxn = num + 0
	} else {
		idx = pos
		pos++
		if (M == "s_order_reversed") idx = nargs - idx + 1
	}
	if (idx > nargs || idx < 1) {
		have = 0
		if (M == "s_missing_stops") stop = 1
		if (M == "s_missing_repeat" && nargs > 0) { have = 1; return args[nargs] }
		return ""
	}
	have = 1
	used = 1
	return args[idx]
}

function padto(s, w, left,   n) {
	n = w - length(s)
	if (n <= 0) return s
	if (left) return s sprintf("%" n "s", "")
	return sprintf("%" n "s", "") s
}

# the bytes of a %b argument: sets bstop when \c was met
function bescape(s,   out, i, c, d, k, v, n) {
	out = ""
	bstop = 0
	i = 1
	n = length(s)
	if (M == "b_raw") return s
	while (i <= n) {
		c = substr(s, i, 1)
		if (c != "\\") { out = out c; i++; continue }
		d = substr(s, i + 1, 1)
		i += 2
		if (d == "\\") out = out ((M == "b_bsl_kept") ? "\\\\" : "\\")
		else if (M == "b_nl_only" && index("abfrtv", d) > 0 && d != "") out = out "\\" d
		else if (d == "a") out = out chr[7]
		else if (d == "b") out = out chr[8]
		else if (d == "f") out = out chr[12]
		else if (d == "n") out = out chr[10]
		else if (d == "r") out = out chr[13]
		else if (d == "t") out = out ((M == "b_t_space") ? " " : chr[9])
		else if (d == "v") out = out chr[11]
		else if (d == "c") {
			if (M == "c_ignored") continue
			if (M == "c_literal") { out = out "\\c"; continue }
			bstop = 1
			sawc = 1
			return out
		} else if (d == "0") {
			k = (M == "b_octal_1") ? 1 : (M == "b_octal_2") ? 2 : (M == "b_octal_4") ? 4 : 3
			if (M == "b_oct_no0") k = 2
			v = 0
			while (k > 0 && isodig(substr(s, i, 1))) {
				v = v * ((M == "b_octal_dec") ? 10 : 8) + substr(s, i, 1)
				i++
				k--
			}
			out = out byte(v)
		} else out = out "\\" d
	}
	return out
}

function byte(v) { v = v % 256; if (v == 0) return sprintf("%c", 0); return chr[v] }

# a literal escape in the format; returns the bytes and advances fi
function fescape(   d, v, k, mx) {
	d = substr(fmt, fi + 1, 1)
	if (d == "") { fi++; return "\\" }
	fi += 2
	if (d == "\\") return (M == "esc_bsl_kept") ? "\\\\" : "\\"
	if (d == "a") return (M == "esc_a_wrong") ? chr[8] : chr[7]
	if (d == "b") return (M == "esc_b_wrong") ? "b" : chr[8]
	if (d == "f") return (M == "esc_f_nl") ? chr[10] : chr[12]
	if (d == "n") return (M == "esc_n_cr") ? chr[13] : chr[10]
	if (d == "r") return (M == "esc_r_nl") ? chr[10] : chr[13]
	if (d == "t") return (M == "esc_t_space") ? " " : chr[9]
	if (d == "v") return (M == "esc_v_wrong") ? chr[12] : chr[11]
	if (isodig(d)) {
		mx = (M == "oct_one") ? 1 : (M == "oct_two") ? 2 : (M == "oct_four") ? 4 : 3
		v = 0
		k = 0
		fi--
		st = fi
		while (k < mx && isodig(substr(fmt, fi, 1))) {
			v = v * ((M == "oct_decimal") ? 10 : 8) + substr(fmt, fi, 1)
			fi++
			k++
		}
		if (M == "oct_needs3" && k < 3) return "\\" substr(fmt, st, k)
		return byte(v)
	}
	return "\\" d
}

# one use of the format
function pass(   c, fl, w, pr, num, conv, j, a, v, s, t, hasp, neg, f, left) {
	fi = 1
	used = 0
	maxn = 0
	endpass = 0
	flen = length(fmt)
	while (fi <= flen && !stop && !endpass) {
		c = substr(fmt, fi, 1)
		if (c == "\\") {
			if (M == "esc_double_pass") {
				t = fescape()
				if (t == "\\" && substr(fmt, fi, 1) == "n") { fi++; t = chr[10] }
				emit(t)
			} else emit(fescape())
			continue
		}
		if (c != "%") {
			if (M == "lit_spaces" && c == " " && lastspace) { fi++; continue }
			if (M == "lit_nl_stops" && c == chr[10]) { stop = 1; continue }
			if (M == "c_keeps_later_args" && stopped) { fi++; continue }
			lastspace = (c == " ")
			emit(c)
			fi++
			continue
		}
		lastspace = 0
		j = fi + 1
		num = ""
		t = j
		while (isdig(substr(fmt, t, 1))) t++
		if (t > j && substr(fmt, t, 1) == "$") {
			num = substr(fmt, j, t - j)
			j = t + 1
			if (M == "n_unsupported") { emit(substr(fmt, fi, j - fi)); fi = j; continue }
		}
		fl = ""
		while (index("-+ #0", substr(fmt, j, 1)) > 0 && substr(fmt, j, 1) != "") {
			fl = fl substr(fmt, j, 1)
			j++
		}
		w = ""
		while (isdig(substr(fmt, j, 1))) { w = w substr(fmt, j, 1); j++ }
		hasp = 0
		pr = ""
		if (substr(fmt, j, 1) == ".") {
			hasp = 1
			j++
			while (isdig(substr(fmt, j, 1))) { pr = pr substr(fmt, j, 1); j++ }
			if (pr == "") pr = 0
		}
		conv = substr(fmt, j, 1)
		fi = j + 1
		if (conv == "%") {
			if (M == "pct_none") continue
			if (M == "pct_double") { emit("%%"); continue }
			if (M == "pct_arg") getarg("")
			emit("%")
			continue
		}
		if (stopped && M == "c_keeps_format") continue
		if (conv == "") { emit("%"); continue }
		if (index("diouxXscb", conv) == 0 && !(M == "locale_radix" && conv == "f")) { emit("%" fl w (hasp ? "." pr : "") conv); continue }
		orig = conv
		a = getarg(num)
		if (!have && M == "s_missing_text" && conv == "s") a = "(null)"
		if (!have && M == "s_missing_d_empty" && index("diouxX", conv) > 0) continue
		if (M == "w_ignored") w = ""
		if (M == "w_minus_ignored") gsub(/-/, "", fl)
		if (M == "w_left_always" && w != "") fl = fl "-"
		left = index(fl, "-") > 0
		if (conv == "s" || conv == "b" || conv == "c") {
			if (M == "s_interprets" && conv == "s") conv = "b"
			if (conv == "s") s = a
			else if (conv == "b") {
				s = bescape(a)
				if (bstop) {
					if (M == "c_arg_only") bstop = 0
					else if (M == "c_pass_only") { pstop = 1; bstop = 0 }
					else if (M == "c_keeps_format" || M == "c_keeps_later_args") { stopped = 1; bstop = 0 }
				}
			} else {
				if (M == "c_all") s = a
				else if (M == "c_last") s = substr(a, length(a), 1)
				else if (M == "c_second") s = substr(a, 2, 1)
				else if (M == "c_none") s = ""
				else if (a == "" && M == "c_empty_space") s = " "
				else if (a == "" && M == "c_empty_null") s = sprintf("%c", 0)
				else s = substr(a, 1, 1)
			}
			if (conv == "s" || conv == "b") {
				if (hasp && M != "p_ignored" && !(M == "p_zero_all" && pr + 0 == 0)) {
					v = pr + ((M == "p_plus1") ? 1 : 0)
					s = substr(s, 1, v)
				}
			}
			if (w != "") {
				if (M == "w_cuts" && length(s) > w + 0) s = substr(s, 1, w + 0)
				s = padto(s, w + 0, left)
			}
			emit(s)
			if (bstop) { stop = 1 }
			if (pstop) { pstop = 0; endpass = 1 }
			continue
		}
		if (conv == "f") {
			t = sprintf("%" fl w (hasp ? "." pr : "") "f", a + 0)
			if (radix() != ".") sub(/\./, radix(), t)
			emit(t)
			continue
		}
		# numeric conversions
		if (M == "d_i_unsupported" && conv == "i") { emit("%i"); continue }
		v = tonum(a)
		if (badnum) {
			diag(a ": not completely converted")
			if (M == "bad_status_last") lastbad = 1
			else badrc = 1
			if (M == "bad_stops") stop = 1
		} else if (M == "bad_status_last") lastbad = 0
		if (conv == "i") conv = "d"
		if (conv == "u") conv = "d"
		f = fl
		if (conv == "d") {
			if (M == "d_blank" && f !~ /[ +]/) f = f " "
			if (M == "fl_plus_ignored") gsub(/\+/, "", f)
			if (M == "fl_space_ignored") gsub(/ /, "", f)
		}
		if (M == "fl_minus_ignored") gsub(/-/, "", f)
		if (M == "fl_zero_ignored") gsub(/0/, "", f)
		if (M == "fl_alt_o_ignored" && conv == "o") gsub(/#/, "", f)
		if (M == "fl_alt_x_ignored" && (conv == "x" || conv == "X")) gsub(/#/, "", f)
		if (!hasp || M == "fl_prec_ignored") { pr = ""; hasp = 0 }
		if (M == "fl_prec_as_width" && hasp && w == "") { w = pr; pr = ""; hasp = 0 }
		if (M == "fl_width_short" && w + 0 > 0) w = w - 1
		if (M == "o_decimal" && conv == "o") conv = "d"
		if (M == "x_decimal" && conv == "x") conv = "d"
		if (M == "x_upper" && conv == "x") conv = "X"
		if (M == "X_lower" && conv == "X") conv = "x"
		if (M == "o_zero_prefix" && conv == "o") f = f "#"
		if (M == "x_prefix" && (conv == "x" || conv == "X")) f = f "#"
		if (M == "u_blank" && orig == "u") f = f " "
		t = sprintf("%" f w (hasp ? "." pr : "") conv, v)
		if (M == "fl_alt_X_lower" && conv == "X" && f ~ /#/) t = "0x" substr(t, 3)
		if (M == "fl_zero_after_sign" && v < 0 && f ~ /0/ && w != "") {
			t = sprintf("%d", v)
			while (length(t) < w + 0) t = "0" t
		}
		emit(t)
	}
}

BEGIN {
	for (i = 1; i < 256; i++) chr[i] = sprintf("%c", i)
	fmt = substr(ARGV[1], 2)
	nargs = ARGC - 2
	for (i = 1; i <= nargs; i++) args[i] = substr(ARGV[i + 1], 2)
	pos = 1
	base = 0
	stop = 0
	badrc = 0
	lastbad = 0
	npass = 0
	do {
		base = pos - 1
		if (M == "n_reuse_base0") base = 0
		pass()
		npass++
		if (maxn > 0) pos = base + maxn + 1
		if (M == "n_reuse_base0" && maxn > 0) pos = (npass) * maxn + 1
		if (M == "s_no_reuse") break
		if (!used && pos > 0 && maxn == 0) break
	} while (pos <= nargs && !stop)
	if (M == "s_extra_pass" && !stop && nargs > 0) pass()
	if (M == "lit_newline") emit(chr[10])
	rc = 0
	if (badrc || lastbad) rc = (M == "bad_status0") ? 0 : 1
	if (M == "lit_status1" || M == "b_status1" || M == "pct_status1") rc = 1
	if (M == "c_status1" && sawc) rc = 1
	if (M == "status2" && rc) rc = 2
	if (M == "status255" && rc) rc = 255
	exit rc
}

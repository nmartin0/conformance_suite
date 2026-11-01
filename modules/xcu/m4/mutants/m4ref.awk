# A small macro processor following the standard's description, with one
# planted fault (F) or one conforming variant (V), chosen by name.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# Driven by m4ref.sh. With neither F nor V it is a plain conforming
# implementation of the part of m4 that the tests exercise.

function fatal(msg) {
	if (F != "fatal_silent") printf "m4: %s\n", msg > "/dev/stderr"
	finish(1)
}
function warn(msg) {
	if (F != "nodiag_all") printf "m4: %s\n", msg > "/dev/stderr"
	ERRS = 1
}
function errstat(   c) {
	c = (V == "err_status2") ? 2 : 1
	return c
}

# ---- output ----
function outp(s) { OBUF = OBUF s }
function flushout() { printf "%s", OBUF; OBUF = "" }
function emit(s) {
	if (CUR < 0) return
	if (CUR == 0) outp(s)
	else DV[CUR] = DV[CUR] s
}
function flushdivs(   i) {
	if (F == "divs_reverse") { for (i = 9; i >= 1; i--) if (DV[i] != "") { outp(DV[i]); DV[i] = "" }; return }
	for (i = 1; i <= 9; i++) if (DV[i] != "") { outp(DV[i]); DV[i] = "" }
}
function finish(code) {
	if (F == "m4exit_discards") { OBUF = "" }
	if (F == "divs_first") { tmp = OBUF; OBUF = ""; flushdivs(); OBUF = OBUF tmp }
	else if (F != "no_flush_divs") flushdivs()
	if (F == "exit_status0") code = 0
	if (ERRS && code == 0 && F != "err_forgotten") code = errstat()
	if (F == "ok_status1" && code == 0) code = 1
	if (F == "final_nl_dropped") sub(/\n$/, "", OBUF)
	if (F == "final_nl_added") OBUF = OBUF "\n"
	flushout()
	exit code
}

# ---- definitions ----
function setdef(nm, text, push,   t, p) {
	if (substr(text, 1, 4) == "\001BI:") {
		p = substr(text, 5, length(text) - 5)
		if (push || SP[nm] == 0) SP[nm]++
		TXT[nm, SP[nm]] = p; ISBI[nm, SP[nm]] = 1
		return
	}
	if (push) {
		SP[nm]++
	} else if (SP[nm] == 0) {
		SP[nm] = 1
	} else if (V == "define_replaces_stack") {
		SP[nm] = 1
	}
	TXT[nm, SP[nm]] = text; ISBI[nm, SP[nm]] = 0
}
function isname(s) { return s ~ /^[A-Za-z_][A-Za-z0-9_]*$/ }

# ---- tokens ----
function startswith(s) { return substr(IN, 1, length(s)) == s }
function readquote(   depth, out, ll, lr, c) {
	ll = length(lq); lr = length(rq)
	IN = substr(IN, ll + 1)
	depth = 1; out = ""
	while (1) {
		if (IN == "") fatal("end of input in quoted string")
		if (startswith(rq)) {
			depth--
			IN = substr(IN, lr + 1)
			if (depth == 0) break
			out = out rq
			continue
		}
		if (startswith(lq)) {
			depth++
			IN = substr(IN, ll + 1)
			out = out lq
			continue
		}
		out = out substr(IN, 1, 1)
		IN = substr(IN, 2)
	}
	if (F == "quote_keep_outer") out = lq out rq
	return out
}
function readcomment(   out, i, ce1) {
	out = cb
	IN = substr(IN, length(cb) + 1)
	ce1 = ce
	if (F == "comment_end_newline") ce1 = "\n"
	i = index(IN, ce1)
	if (i == 0) { out = out IN; IN = ""; return out }
	out = out substr(IN, 1, i - 1 + length(ce1))
	IN = substr(IN, i + length(ce1))
	return out
}
function readuntil(e,   i, r) {
	i = index(IN, e)
	if (i == 0) { r = IN; IN = ""; return r }
	r = substr(IN, 1, i - 1 + length(e)); IN = substr(IN, i + length(e)); return r
}
# sets TT (Q quote, C comment, N name, O other) and TV
function tok(   c, i, n, nm, cbx, lqx) {
	cbx = cb
	if (F == "comment_scanned") cbx = ""
	if (F == "changecom_keeps_hash" && cbx != "#" && substr(IN, 1, 1) == "#") { TT = "C"; IN = substr(IN, 2); TV = "#" readuntil("\n"); return }
	lqx = lq
	if (F == "quote_ignored") lqx = ""
	if (lqx != "" && startswith(lqx)) { TT = "Q"; TV = readquote(); return }
	if (cbx != "" && substr(IN, 1, length(cbx)) == cbx) {
		if (F == "comment_scanned") { }
		TT = "C"; TV = readcomment(); return
	}
	c = substr(IN, 1, 1)
	if (c ~ /[A-Za-z_]/ || (F == "name_digit_start" && c ~ /[0-9]/)) {
		n = length(IN); i = 2
		while (i <= n) {
			c = substr(IN, i, 1)
			if (F == "name_no_underscore") { if (c !~ /[A-Za-z0-9]/) break }
			else if (F == "name_no_digit") { if (c !~ /[A-Za-z_]/) break }
			else if (c !~ /[A-Za-z0-9_]/) break
			i++
		}
		TT = "N"; TV = substr(IN, 1, i - 1); IN = substr(IN, i)
		if (F == "name_shortest") { IN = substr(TV, 2) IN; TV = substr(TV, 1, 1) }
		return
	}
	TT = "O"; TV = c; IN = substr(IN, 2)
}

# ---- argument collection ----
function collect(fr,   depth, cur, k, c) {
	k = 1; depth = 0; cur = ""
	while (1) {
		if (IN == "") fatal("end of input in argument list")
		tok()
		if (TT == "Q") { cur = cur TV; continue }
		if (TT == "C") { cur = cur TV; continue }
		if (TT == "N") {
			if (F == "args_not_expanded") { cur = cur TV; continue }
			if (!expandname(TV)) cur = cur TV
			continue
		}
		c = TV
		if (c == "\001") continue
		if (c == "(") { depth++; cur = cur c; continue }
		if (c == ")") {
			if (depth == 0) break
			depth--; cur = cur c; continue
		}
		if (c == "," && (depth == 0 || F == "paren_split")) {
			A[fr, k] = cur; k++; cur = ""; continue
		}
		if (c ~ /[ \t\n]/ && cur == "") {
			if (F == "lead_ws_kept") { cur = cur c }
			else if (F == "lead_ws_space_only" && c != " ") { cur = cur c }
			continue
		}
		cur = cur c
	}
	if (F == "trail_ws_dropped") sub(/[ \t\n]+$/, "", cur)
	A[fr, k] = cur; N[fr] = k
	if (F == "args_split_after_expand") { }
}
function arg(fr, i) {
	if (i < 1 || i > N[fr]) return ""
	return A[fr, i]
}
# defining text of a built-in: rescanned with the input that follows
function ret(s) { IN = s IN }
# a literal result that is not scanned again
function lit(s) { IN = lq s rq IN }

function expandname(nm,   top, fr, hasp, res, i, c, d, n, L, text, isb, cnt) {
	if (SP[nm] == 0) return 0
	top = SP[nm]
	isb = ISBI[nm, top]
	hasp = (substr(IN, 1, 1) == "(")
	if (isb && (nm in NEEDARG) && !hasp) return 0
	if (F == "paren_after_space" && !hasp) { }
	fr = ++FR
	N[fr] = 0
	if (hasp) { IN = substr(IN, 2); collect(fr) }
	if (TRACEALL || TRACE[nm]) trace(nm, fr, hasp)
	if (isb) { builtin(TXT[nm, top], fr, hasp); FR = fr - 1; return 1 }
	text = TXT[nm, top]
	cnt = hasp ? N[fr] : 0
	res = ""
	L = length(text)
	i = 1
	while (i <= L) {
		c = substr(text, i, 1)
		if (c == "$" && i < L) {
			d = substr(text, i + 1, 1)
			if (d ~ /[0-9]/) {
				n = d + 0
				if (n == 0) {
					if (F != "dollar0_empty") res = res nm
				} else {
					if (F == "arg_off_by_one") n = n + 1
					if (F == "max_args_8" && n == 9) res = res
					else if (F == "arg_swap12" && n <= 2) res = res arg(fr, 3 - n)
					else res = res arg(fr, n)
				}
				i += 2; continue
			}
			if (d == "#") {
				n = cnt
				if (F == "hash_noparen_1" && !hasp) n = 1
				if (F == "hash_plus1") n = n + 1
				if (F == "hash_empty_zero" && hasp && n == 1 && A[fr, 1] == "") n = 0
				if (F == "hash_cap9" && n > 9) n = 9
				if (F == "hash_commas_only") { }
				res = res n; i += 2; continue
			}
			if (d == "*") {
				res = res joinargs(fr, cnt, 0)
				i += 2; continue
			}
			if (d == "@") {
				res = res joinargs(fr, cnt, 1)
				i += 2; continue
			}
		}
		res = res c; i++
	}
	FR = fr - 1
	if (F == "no_rescan") lit(res)
	else if (F == "rescan_isolated") ret(res "\001")
	else ret(res)
	return 1
}
function joinargs(fr, cnt, quoted,   j, s, sep, a) {
	sep = ","
	if (!quoted && F == "star_space") sep = ", "
	if (quoted && F == "at_space") sep = ", "
	if (!quoted && F == "star_semicolon") sep = ";"
	s = ""
	for (j = 1; j <= cnt; j++) {
		a = A[fr, j]
		if (quoted && F != "at_unquoted") a = lq a rq
		if (!quoted && F == "star_quoted") a = lq a rq
		s = s (j > 1 ? sep : "") a
	}
	if (!quoted && F == "star_first_only") s = (cnt >= 1) ? A[fr, 1] : ""
	return s
}
function trace(nm, fr, hasp,   j, s) {
	if (F == "trace_nothing") return
	s = ""
	for (j = 1; j <= (hasp ? N[fr] : 0); j++) s = s (j > 1 ? "," : "") A[fr, j]
	if (V == "trace_format") printf "TRACE<%s>[%s]\n", nm, s > "/dev/stderr"
	else if (F == "trace_stdout") outp("m4trace: " nm "(" s ")\n")
	else printf "m4trace: %s(%s)\n", nm, s > "/dev/stderr"
}

# ---- numbers ----
function isnum(s) { return s ~ /^-?[0-9]+$/ }

# ---- eval ----
function evprec(op) {
	return P[op]
}
function evtoken(   c, n, s, d, i, base, v, ch) {
	while (substr(EV, EP, 1) ~ /[ \t\n]/) EP++
	if (EP > length(EV)) { ET = "E"; return }
	c = substr(EV, EP, 1)
	if (c ~ /[0-9]/) {
		s = substr(EV, EP)
		if (s ~ /^0[xX][0-9a-fA-F]+/ && F != "eval_hex_missing" && (F != "eval_hex_upperx" || substr(s,2,1) == "x")) {
			match(s, /^0[xX][0-9a-fA-F]+/)
			d = substr(s, 3, RLENGTH - 2); EP += RLENGTH
			v = 0
			for (i = 1; i <= length(d); i++) {
				ch = index("0123456789abcdef", tolower(substr(d, i, 1))) - 1
				v = v * 16 + ch
			}
			ET = "N"; EN = v; return
		}
		match(s, /^[0-9]+/)
		d = substr(s, 1, RLENGTH); EP += RLENGTH
		if (length(d) > 1 && substr(d, 1, 1) == "0" && F != "eval_octal_decimal") {
			v = 0
			for (i = 2; i <= length(d); i++) v = v * 8 + substr(d, i, 1)
			ET = "N"; EN = v; return
		}
		ET = "N"; EN = d + 0; return
	}
	s = substr(EV, EP, 2)
	if (s == "<<" || s == ">>" || s == "<=" || s == ">=" || s == "==" || s == "!=" || s == "&&" || s == "||") {
		ET = "O"; EO = s; EP += 2; return
	}
	ET = "O"; EO = c; EP++
}
function evparse(minp,   left, op, p, right, u) {
	left = evunary()
	while (1) {
		save = EP
		evtoken()
		if (ET != "O" || !(EO in P)) { EP = save; return left }
		op = EO; p = P[op]
		if (p < minp) { EP = save; return left }
		right = evparse((F == "eval_right_assoc" && (op == "-" || op == "/")) ? p : p + 1)
		left = evapply(op, left, right)
	}
}
function evunary(   v, op) {
	evtoken()
	if (ET == "N") return EN
	if (ET != "O") { EERR = 1; return 0 }
	op = EO
	if (op == "(") {
		v = evparse(0)
		evtoken()
		if (!(ET == "O" && EO == ")")) EERR = 1
		return v
	}
	if (op == "-") { v = evunaryarg(); return -v }
	if (op == "+") {
		if (F == "eval_unary_plus_missing") { EERR = 1; return 0 }
		return evunaryarg()
	}
	if (op == "~") {
		v = evunaryarg()
		if (F == "eval_tilde_neg") return -v
		return -v - 1
	}
	if (op == "!") {
		v = evunaryarg()
		if (F == "eval_not_bitwise") return -v - 1
		if (F == "eval_not_neg") return (v == 0) ? -1 : 0
		return (v == 0) ? 1 : 0
	}
	EERR = 1; return 0
}
function evunaryarg(   v, sv) {
	if (F == "eval_bang_low" || F == "eval_tilde_low") {
		# the historic mistake: the unary operator takes the whole comparison
		return evparse(P["=="])
	}
	return evunary()
}
function tou(x) { x = x % 4294967296; if (x < 0) x += 4294967296; return x }
function tos(x) { if (x >= 2147483648) x -= 4294967296; return x }
function bitop(op, a, b,   r, bit, i, x, y) {
	a = tou(a); b = tou(b); r = 0; bit = 1
	for (i = 0; i < 32; i++) {
		x = a % 2; y = b % 2
		a = (a - x) / 2; b = (b - y) / 2
		if ((op == "&" && x && y) || (op == "|" && (x || y)) || (op == "^" && (x != y))) r += bit
		bit *= 2
	}
	return tos(r)
}
function evapply(op, a, b,   r) {
	if (op == "*") r = a * b
	else if (op == "/") {
		if (b == 0) { EERR = 1; return 0 }
		if (F == "eval_floor_div") { r = a / b; if (r != int(r) && r < 0) r = int(r) - 1; else r = int(r) }
		else r = int(a / b)
	}
	else if (op == "%") {
		if (b == 0) { EERR = 1; return 0 }
		if (F == "eval_floor_mod") { r = a - b * int(a / b); if (r != 0 && ((r < 0) != (b < 0))) r += b }
		else r = a - b * int(a / b)
	}
	else if (op == "+") r = a + b
	else if (op == "-") r = a - b
	else if (op == "<<") { r = a; while (b-- > 0) r *= 2 }
	else if (op == ">>") { r = a; while (b-- > 0) r = int(r / 2) - ((r < 0 && r % 2 != 0) ? 1 : 0) }
	else if (op == "<") r = a < b
	else if (op == "<=") r = (F == "eval_le_as_lt") ? a < b : a <= b
	else if (op == ">") r = a > b
	else if (op == ">=") r = (F == "eval_ge_as_gt") ? a > b : a >= b
	else if (op == "==") r = a == b
	else if (op == "!=") r = (F == "eval_ne_as_eq") ? a == b : a != b
	else if (op == "&") r = bitop("&", a, b)
	else if (op == "^") r = bitop("^", a, b)
	else if (op == "|") r = bitop("|", a, b)
	else if (op == "&&") {
		if (F == "eval_logical_value") r = (a != 0 && b != 0) ? b : 0
		else r = (a != 0 && b != 0)
	}
	else if (op == "||") {
		if (F == "eval_logical_value") r = (a != 0) ? a : (b != 0 ? b : 0)
		else r = (a != 0 || b != 0)
	}
	if (F == "eval_wrap16") { r = r % 65536; if (r >= 32768) r -= 65536; if (r < -32768) r += 65536 }
	return r + 0
}
function setprec() {
	P["||"] = 10; P["&&"] = 20; P["|"] = 30; P["^"] = 40; P["&"] = 50
	P["=="] = 60; P["!="] = 60; P["<"] = 70; P["<="] = 70; P[">"] = 70; P[">="] = 70
	P["<<"] = 80; P[">>"] = 80; P["+"] = 90; P["-"] = 90; P["*"] = 100; P["/"] = 100; P["%"] = 100
	if (F == "eval_flat_muldiv_add") { P["*"] = 90; P["/"] = 90; P["%"] = 90 }
	if (F == "eval_shift_above_add") { P["<<"] = 95; P[">>"] = 95 }
	if (F == "eval_eq_with_rel") { P["=="] = 70; P["!="] = 70 }
	if (F == "eval_or_with_xor") { P["|"] = 40 }
	if (F == "eval_xor_above_and") { P["^"] = 50; P["&"] = 40 }
	if (F == "eval_andand_with_oror") { P["&&"] = 10 }
	if (F == "eval_and_above_eq") { P["&"] = 65 }
	if (F == "eval_and_with_or") { P["&"] = 30 }
}
function digitchar(d) { return substr(V == "eval_upper" ? "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ" : "0123456789abcdefghijklmnopqrstuvwxyz", d + 1, 1) }
function fmtnum(v, radix, width,   neg, s, d) {
	neg = (v < 0); if (neg) v = -v
	s = ""
	if (v == 0) s = "0"
	while (v > 0) { d = v % radix; s = digitchar(d) s; v = (v - d) / radix }
	if (F != "eval_width_ignored") {
		if (F == "eval_width_trunc") { if (length(s) > width && width > 0) s = substr(s, length(s) - width + 1) }
		while (length(s) < width) s = ((F == "eval_width_space") ? " " : "0") s
	}
	return (neg ? "-" : "") s
}

# ---- built-ins ----
function builtin(b, fr, hasp,   a1, a2, a3, a4, i, n, s, j, k, t, nm, m, e, rx, w, hd, lo, hi, ok, pos) {
	a1 = arg(fr, 1); a2 = arg(fr, 2); a3 = arg(fr, 3); a4 = arg(fr, 4)
	n = N[fr]
	if (b == "define" || b == "pushdef") {
		if (n >= 1) {
			if (F == "define_returns_name") lit(a1)
			if (F == "define_returns_text") lit(a2)
			if (F == "pushdef_returns_name" && b == "pushdef") lit(a1)
			if (F == "pushdef_returns_text" && b == "pushdef") lit(a2)
			if (b == "pushdef" && F == "pushdef_replaces") setdef(a1, a2, 0)
			else if (b == "define" && F == "define_noreplace" && SP[a1] > 0) { }
			else if (b == "define" && F == "define_pushes") setdef(a1, a2, 1)
			else if (b == "pushdef" && F == "pushdef_ignored") { }
			else if (b == "define" && F == "define_ignored") { }
			else if (b == "define" && F == "define_second_empty") setdef(a1, "", 0)
			else setdef(a1, a2, b == "pushdef")
		}
		return
	}
	if (b == "popdef") {
		for (i = 1; i <= n; i++) {
			nm = A[fr, i]
			if (F == "popdef_all") SP[nm] = 0
			else if (F == "popdef_noop") { }
			else if (SP[nm] > 0) SP[nm]--
			if (F == "popdef_returns_name") lit(nm)
			if (F == "popdef_first_only") break
		}
		return
	}
	if (b == "undefine") {
		for (i = 1; i <= n; i++) {
			nm = A[fr, i]
			if (F == "undefine_top_only") { if (SP[nm] > 0) SP[nm]-- }
			else if (F == "undefine_noop") { }
			else SP[nm] = 0
			if (F == "undefine_returns_name") lit(nm)
			if (F == "undefine_first_only") break
		}
		return
	}
	if (b == "ifdef") {
		k = (SP[a1] > 0)
		if (F == "ifdef_inverted") k = !k
		if (F == "ifdef_empty_undefined" && k && !ISBI[a1, SP[a1]] && TXT[a1, SP[a1]] == "") k = 0
		if (F == "ifdef_nobuiltin" && k && ISBI[a1, SP[a1]]) k = 0
		if (F == "ifdef_always_second") k = 1
		if (k) ret(a2)
		else if (F == "ifdef_undef_emits_name") lit(a1)
		else if (n >= 3) ret(a3)
		return
	}
	if (b == "ifelse") {
		i = 1
		while (1) {
			m = n - i + 1
			if (m < 3) { if (m == 2 && F == "ifelse_two_args_second") ret(A[fr, i + 1]); return }
			x = A[fr, i]; y = A[fr, i + 1]
			eq = (x == y)
			if (F == "ifelse_prefix") { eq = (substr(x, 1, length(y)) == y || substr(y, 1, length(x)) == x) }
			if (F == "ifelse_case") eq = (tolower(x) == tolower(y))
			if (F == "ifelse_trim") { u = x; v = y; sub(/[ \t]+$/, "", u); sub(/[ \t]+$/, "", v); eq = (u == v) }
			if (F == "ifelse_numeric" && x ~ /^-?[0-9]+$/ && y ~ /^-?[0-9]+$/) eq = (x + 0 == y + 0)
			if (F == "ifelse_length" ) eq = (length(x) == length(y))
			if (eq) { ret(A[fr, i + 2]); return }
			if (m == 3) { if (F == "ifelse_nonequal_third") ret(A[fr, i + 2]); return }
			if (m <= 5) {
				if (F == "ifelse_5args_fifth" && m == 5) ret(A[fr, i + 4])
				else if (F == "ifelse_4args_empty" && m == 4) { }
				else ret(A[fr, i + 3])
				return
			}
			if (F == "ifelse_6_discard2") i += 2
			else if (F == "ifelse_6_stop") { ret(A[fr, i + 3]); return }
			else if (F == "ifelse_6_last") { ret(A[fr, n]); return }
			else if (F == "ifelse_6_discard4") i += 4
			else i += 3
		}
	}
	if (b == "incr" || b == "decr") {
		s = a1 + 0
		if (b == "incr") {
			if (F == "incr_2") s += 2
			else if (F == "incr_abs") s = (s < 0 ? -s : s) + 1
			else if (F == "incr_nocarry") { lo = a1 % 10; hd = int(a1 / 10); s = hd * 10 + (lo + 1) % 10 }
			else if (F == "incr_ident") { }
			else s += 1
		} else {
			if (F == "decr_2") s -= 2
			else if (F == "decr_clamp0") s = (s > 0) ? s - 1 : 0
			else if (F == "decr_ident") { }
			else if (F == "decr_is_incr") s += 1
			else s -= 1
		}
		if (F == "incr_has_plus" && b == "incr" && s > 0) { ret("+" s); return }
		if (F == "incr_nonum_err" ) { }
		ret(s ""); return
	}
	if (b == "len") {
		s = length(a1)
		if (F == "len_plus1") s++
		else if (F == "len_stops_at_space") { i = index(a1, " "); if (i > 0) s = i - 1 }
		else if (F == "len_empty_one" && a1 == "") s = 1
		else if (F == "len_minus1" && s > 0) s--
		ret(s ""); return
	}
	if (b == "index") {
		if (F == "index_last") {
			pos = -1
			for (i = 1; i + length(a2) - 1 <= length(a1); i++) if (substr(a1, i, length(a2)) == a2) pos = i - 1
			ret(pos ""); return
		}
		i = index(a1, a2)
		if (F == "index_firstchar" && a2 != "") { i = index(a1, substr(a2, 1, 1)) }
		if (F == "index_prefix_only") i = (substr(a1, 1, length(a2)) == a2) ? 1 : 0
		pos = i - 1
		if (F == "index_1origin" && i > 0) pos = i
		if (F == "index_missing_0" && i == 0) pos = 0
		if (F == "index_missing_len" && i == 0) pos = length(a1)
		ret(pos ""); return
	}
	if (b == "substr") {
		s = a2 + 0
		if (F == "substr_1origin") s = s - 1
		if (s >= length(a1)) { if (F == "substr_beyond_err") warn("substr: start beyond end"); return }
		if (n >= 3 && !(F == "substr_third_zero_all" && a3 + 0 == 0) && !(F == "substr_third_ignored")) {
			k = a3 + 0
			if (F == "substr_len_is_end") k = k - s
			if (F == "substr_len_plus1") k++
			ret(substr(a1, s + 1, k)); return
		}
		ret(substr(a1, s + 1)); return
	}
	if (b == "translit") {
		s = ""
		for (i = 1; i <= length(a1); i++) {
			c = substr(a1, i, 1)
			j = index(a2, c)
			if (F == "translit_last_match") { j = 0; for (k = 1; k <= length(a2); k++) if (substr(a2, k, 1) == c) j = k }
			if (j == 0) { s = s c; continue }
			if (F == "translit_first_only" && j != 1) { s = s c; continue }
			if (F == "translit_reverse") { k = index(a3, c); if (k > 0) s = s substr(a2, k, 1); else s = s c; continue }
			if (j <= length(a3)) s = s substr(a3, j, 1)
			else if (F == "translit_pad_last" && length(a3) > 0) s = s substr(a3, length(a3), 1)
			else if (F == "translit_nodelete") s = s c
		}
		if (F == "translit_noop") s = a1
		ret(s); return
	}
	if (b == "shift") {
		s = ""
		hd = 2
		if (F == "shift_drop2") hd = 3
		if (F == "shift_keep_first") hd = 1
		for (i = hd; i <= n; i++) {
			t = A[fr, i]
			if (F != "shift_unquoted") t = lq t rq
			s = s (i > hd ? (F == "shift_space" ? ", " : ",") : "") t
		}
		ret(s); return
	}
	if (b == "defn") {
		s = ""
		for (i = 1; i <= n; i++) {
			nm = A[fr, i]
			if (SP[nm] == 0) continue
			if (ISBI[nm, SP[nm]]) t = "\001BI:" TXT[nm, SP[nm]] "\001"
			else t = TXT[nm, SP[nm]]
			if (F == "defn_unquoted") s = s t
			else s = s lq t rq
		}
		if (F == "defn_name") s = lq a1 rq
		ret(s); return
	}
	if (b == "eval") {
		EV = a1; EP = 1; EERR = 0
		v = evparse(0)
		evtoken()
		if (ET != "E") EERR = 1
		if (EERR) { warn("eval: bad expression"); return }
		rx = 10
		if (n >= 2 && a2 != "") rx = a2 + 0
		if (F == "eval_radix_ignored") rx = 10
		w = 0
		if (n >= 3 && a3 != "") w = a3 + 0
		if (F == "eval_blank_radix_error" && n >= 2 && a2 == "") { warn("eval: bad radix"); return }
		if (F == "eval_radix_is_width" && n >= 2) { w = a2 + 0; rx = 10 }
		ret(fmtnum(v, rx, w)); return
	}
	if (b == "divert") {
		if (F == "divert_noarg_ignored" && n == 0) return
		k = (n >= 1 && a1 != "") ? a1 + 0 : 0
		if (F == "divert_noarg_stays" && n == 0) return
		if (F == "divert_neg_only1" && k < -1) k = 0
		if (F == "divert_neg_stdout" && k < 0) k = 0
		if (F == "divert_max8" && k == 9) k = -1
		if (F == "divert_9_is_1" && k == 9) k = 1
		if (F == "divert_ignored") return
		CUR = k < 0 ? -1 : k
		if (k > 9) CUR = -1
		if (F == "divert_returns_num") lit(k "")
		return
	}
	if (b == "divnum") {
		k = CUR
		if (F == "divnum_zero") k = 0
		if (F == "divnum_neg_zero" && k < 0) k = 0
		if (F == "divnum_plus1") k = k + 1
		ret(k ""); return
	}
	if (b == "undivert") {
		if (F == "undivert_ignored") return
		if (n == 0) {
			if (F == "undivert_noargs_none") return
			if (F == "undivert_all_reverse") { for (i = 9; i >= 1; i--) undiv(i) }
			else for (i = 1; i <= 9; i++) undiv(i)
		} else {
			for (i = 1; i <= n; i++) {
				undiv(A[fr, i] + 0)
				if (F == "undivert_first_only") break
			}
		}
		return
	}
	if (b == "dnl") {
		if (F == "dnl_noop") return
		i = index(IN, "\n")
		if (F == "dnl_keeps_newline") { IN = (i == 0) ? "" : substr(IN, i); return }
		if (F == "dnl_word_only") { match(IN, /^[ \t]*[^ \t\n]*/); IN = substr(IN, RLENGTH + 1); return }
		if (F == "dnl_stops_at_comma") { i = index(IN, ","); IN = (i == 0) ? "" : substr(IN, i + 1); return }
		IN = (i == 0) ? "" : substr(IN, i + 1)
		return
	}
	if (b == "changequote") {
		if (F == "changequote_ignored") return
		if (n == 0) {
			if (F == "changequote_noarg_noop") return
			if (F == "changequote_noarg_disable") { lq = ""; rq = ""; return }
			lq = "`"; rq = "'"; return
		}
		lq = a1
		if (F == "changequote_trunc3") lq = substr(a1, 1, 3)
		if (F == "changequote_trunc1") lq = substr(a1, 1, 1)
		if (n >= 2) rq = a2
		if (F == "changequote_right_unchanged") rq = "'"
		if (F == "changequote_swap") { t = lq; lq = rq; rq = t }
		if (F == "changequote_rtrunc4") rq = substr(rq, 1, 4)
		if (F == "changequote_ltrunc4") lq = substr(lq, 1, 4)
		return
	}
	if (b == "changecom") {
		if (F == "changecom_ignored") return
		if (n == 0) {
			if (F == "changecom_noarg_noop") return
			cb = ""; ce = "\n"; return
		}
		cb = a1
		if (F == "changecom_btrunc3") cb = substr(a1, 1, 3)
		if (F == "changecom_btrunc4") cb = substr(a1, 1, 4)
		if (n >= 2 && a2 != "") ce = a2; else ce = "\n"
		if (F == "changecom_end_ignored") ce = "\n"
		if (F == "changecom_etrunc4") ce = substr(ce, 1, 4)
		if (F == "changecom_etrunc3") ce = substr(ce, 1, 3)
		return
	}
	if (b == "include" || b == "sinclude") {
		if (F == "sinclude_never" && b == "sinclude") return
		if (F == "include_never" && b == "include") return
		s = readfile(a1)
		if (s == "\001NOFILE") {
			if (b == "sinclude" && F != "sinclude_errors" && F != "sinclude_diag") return
			if (b == "include" && F == "include_silent_ok") return
			if (F != "include_no_diag" || b == "sinclude") warn("cannot open " a1)
			INCERR = 1
			if (F == "include_status0" || (F == "sinclude_errors" && b == "sinclude" && 0)) ERRS = 0
			if (F == "sinclude_errors" && b == "sinclude") ERRS = 1
			if (V == "include_abort" && b == "include") finish(errstat())
			if (F == "include_aborts" && b == "include") finish(errstat())
			return
		}
		if (F == "include_empty") return
		if (F == "include_no_final_newline" ) { sub(/\n$/, "", s) }
		if (F == "include_unscanned") lit(s); else ret(s)
		return
	}
	if (b == "syscmd") {
		if (V != "syscmd_noflush") flushout()
		fflush()
		if (F == "syscmd_noop") return
		if (F == "syscmd_exec_direct") {
			rc = system("exec " (a1 ~ /[|;&<>$]/ ? "false" : a1))
			SYSVAL = rc + 0; return
		}
		if (F == "syscmd_stdout_err") { rc = system("(" a1 ") >&2"); SYSVAL = rc + 0; return }
		if (F == "syscmd_stderr_out") { rc = system("(" a1 ") 2>&1"); SYSVAL = rc + 0; return }
		if (F == "syscmd_stderr_dropped") { rc = system("(" a1 ") 2>/dev/null"); SYSVAL = rc + 0; return }
		if (F == "syscmd_rescanned") {
			s = ""
			cmd = a1
			while ((cmd | getline ln) > 0) s = s ln "\n"
			SYSVAL = close(cmd) + 0
			ret(s); return
		}
		if (V == "syscmd_captured") {
			s = ""
			cmd = "(" a1 ")"
			while ((cmd | getline ln) > 0) s = s ln "\n"
			SYSVAL = close(cmd) + 0
			lit(s); return
		}
		rc = system(a1)
		nv = rc + 0
		if (F == "sysval_sticky" && nv == 0) nv = SYSVAL
		SYSVAL = nv
		if (F == "sysval_only_first" && SYSVAL_SET) SYSVAL = FIRSTVAL
		if (!SYSVAL_SET) FIRSTVAL = SYSVAL
		SYSVAL_SET = 1
		return
	}
	if (b == "sysval") {
		k = SYSVAL
		if (F == "sysval_zero") k = 0
		if (F == "sysval_one") k = (k != 0)
		if (F == "sysval_mod128") k = k % 128
		if (F == "sysval_times256") k = k * 256
		ret(k ""); return
	}
	if (b == "m4exit") {
		k = (n >= 1 && a1 != "") ? a1 + 0 : 0
		if (F == "m4exit_noarg_1" && n == 0) k = 1
		if (F == "m4exit_ignores_arg") k = 0
		if (F == "m4exit_clamp127" && k > 127) k = 127
		if (F == "m4exit_mod16") k = k % 16
		if (F == "m4exit_error_forces1" && ERRS) k = 1
		if (F == "m4exit_continues") return
		if (F == "m4exit_zero_after_error" && k == 0) { ERRS = 0 }
		if (F == "m4exit_ignores_operand_err" && k == 0 && OPERR) ERRS = 0
		if (F == "m4exit_ignores_mkstemp_err" && k == 0 && MKERR) ERRS = 0
		if (F == "m4exit_ignores_include_err" && k == 0 && INCERR) ERRS = 0
		if (F == "m4exit_zero_arg_only" && k == 0 && n >= 1) ERRS = 0
		if (F == "m4exit_noarg_only" && k == 0 && n == 0) ERRS = 0
		finish(k)
	}
	if (b == "errprint") {
		if (F == "errprint_nothing") return
		s = ""
		for (i = 1; i <= n; i++) s = s (i > 1 ? " " : "") A[fr, i]
		if (F == "errprint_stdout") { outp(s); return }
		if (F == "errprint_both") { outp(s) }
		if (F == "errprint_first_only") s = a1
		if (F == "errprint_trunc") s = substr(s, 1, length(s) - 1)
		printf "%s", s > "/dev/stderr"
		if (F == "errprint_returns_text") lit(s)
		return
	}
	if (b == "dumpdef") {
		if (F == "dumpdef_nothing") return
		if (n == 0) {
			for (nm in SP) if (SP[nm] > 0) dumpone(nm)
		}
		for (i = 1; i <= n; i++) dumpone(A[fr, i])
		return
	}
	if (b == "m4wrap") {
		if (F == "m4wrap_ignored") return
		NW++; WRAP[NW] = a1
		if (F == "m4wrap_unprocessed") WRAP[NW] = lq a1 rq
		if (F == "m4wrap_returns_text") lit(a1)
		return
	}
	if (b == "mkstemp") { mkstemp(a1); return }
	if (b == "traceon") {
		if (F == "traceon_noop") return
		if (n == 0) { if (F != "traceon_noargs_noop") TRACEALL = 1 }
		for (i = 1; i <= n; i++) TRACE[A[fr, i]] = 1
		if (F == "traceon_returns_text") lit(a1)
		return
	}
	if (b == "traceoff") {
		if (F == "traceoff_noop") return
		if (n == 0) { if (F != "traceoff_noargs_noop") { TRACEALL = 0; for (nm in TRACE) TRACE[nm] = 0 } }
		for (i = 1; i <= n; i++) TRACE[A[fr, i]] = 0
		if (F == "traceoff_first_only") { }
		return
	}
}
function dumpone(nm,   top) {
	if (SP[nm] == 0) return
	top = SP[nm]
	if (V == "trace_format") printf "DEF<%s>\t%s\n", nm, TXT[nm, top] > "/dev/stderr"
	else if (F == "dumpdef_stdout") outp(nm ": " TXT[nm, top] "\n")
	else if (F == "dumpdef_name_only") printf "%s:\n", nm > "/dev/stderr"
	else printf "%s: %s\n", nm, TXT[nm, top] > "/dev/stderr"
}
function undiv(k,   t) {
	if (k < 1 || k > 9 || k == CUR) return
	t = DV[k]
	if (F == "undivert_keeps") { }
	else DV[k] = ""
	if (F == "undivert_rescan") { ret(t); return }
	if (F == "undivert_to_stdout") { outp(t); return }
	if (F == "undivert_adds_newline" && t != "") t = t "\n"
	emit(t)
}
function readfile(path,   s, ln, r) {
	s = ""
	if (path == "") return "\001NOFILE"
	while ((r = (getline ln < path)) > 0) s = s ln "\n"
	if (r < 0) return "\001NOFILE"
	close(path)
	return s
}
function mkfail(tmpl, why) {
	if (F != "mkstemp_fail_no_diag") warn("mkstemp: " why)
	else ERRS = 1
	MKERR = 1
	if (F == "mkstemp_fail_status0") ERRS = 0
	if (F == "mkstemp_fail_text") lit(tmpl)
	if (F == "mkstemp_fail_abort") finish(errstat())
}
function mkstemp(tmpl,   n, pre, i, name, ok, chars, cs, tries) {
	n = length(tmpl)
	ok = (substr(tmpl, n - 5) == "XXXXXX")
	if (!ok) { mkfail(tmpl, "bad template"); return }
	pre = substr(tmpl, 1, n - 6)
	chars = (V == "mkstemp_digits") ? "0123456789" : "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789"
	cs = length(chars)
	for (tries = 0; tries < 200; tries++) {
		name = pre
		for (i = 0; i < 6; i++) name = name substr(chars, int(rand() * cs) + 1, 1)
		if (F == "mkstemp_fixed") name = pre "aaaaaa"
		if (F == "mkstemp_template") name = tmpl
		if (system("(set -C; : >'" name "') 2>/dev/null") == 0) {
			if (F == "mkstemp_nocreate") system("rm -f '" name "'")
			if (F == "mkstemp_dir_dropped") { sub(/^.*\//, "", name) }
			if (F == "mkstemp_prefix_dropped") { name = substr(name, length(pre) + 1) }
			if (F == "mkstemp_short") name = substr(name, 1, length(name) - 1)
			if (F == "mkstemp_nl") name = name "\n"
			lit(name)
			return
		}
	}
	mkfail(tmpl, "cannot create")
}

# ---- driver ----
function run(   t) {
	while (IN != "") {
		tok()
		if (TT == "Q" || TT == "C") { emit(TV); continue }
		if (TT == "N") { if (!expandname(TV)) emit(TV); continue }
		if (TV != "\001") emit(TV)
	}
}
function processfile(path,   s) {
	if (path == "-") {
		s = ""
		while ((getline ln < "/dev/stdin") > 0) s = s ln "\n"
		IN = s
	} else {
		s = readfile(path)
		if (s == "\001NOFILE") {
			if (F != "operand_no_diag") warn("cannot open " path)
			OPERR = 1
			if (F == "operand_status0") ERRS = 0
			if (F == "operand_aborts") finish(errstat())
			return
		}
		IN = s
	}
	if (SYNC) {
		if (V == "sync_nofile") emit("#line 1\n")
		else emit("#line 1 \"" (path == "-" ? "stdin" : path) "\"\n")
	}
	run()
}
BEGIN {
	F = F ""; V = V ""
	lq = "`"; rq = "'"; cb = "#"; ce = "\n"
	CUR = 0; FR = 0; SYSVAL = 0
	srand(PID + 0)
	setprec()
	nb = split("define pushdef popdef undefine ifdef ifelse incr decr len index substr translit shift defn eval divert divnum undivert dnl changequote changecom include sinclude syscmd sysval m4exit errprint m4wrap mkstemp traceon traceoff dumpdef", BL, " ")
	for (i = 1; i <= nb; i++) { SP[BL[i]] = 1; TXT[BL[i], 1] = BL[i]; ISBI[BL[i], 1] = 1 }
	nn = split("define pushdef popdef undefine ifdef ifelse incr decr len index substr translit shift defn eval include sinclude syscmd m4wrap mkstemp errprint", NA, " ")
	for (i = 1; i <= nn; i++) NEEDARG[NA[i]] = 1
	if (F == "dnl_needs_parens") { }
	# options, in the order given
	nf = 0
	if (F == "s_always") SYNC = 1
	while ((getline ln < OPS) > 0) {
		k = substr(ln, 1, 1); v = substr(ln, 3)
		if (k == "S") { SYNC = (F != "s_ignored"); continue }
		if (k == "T") { continue }
		if (k == "F") { FILES[++nf] = v; continue }
		if (k == "D") { OPL[++no] = "D" v; continue }
		if (k == "U") { OPL[++no] = "U" v; continue }
	}
	if (F == "U_before_D") {
		for (i = 1; i <= no; i++) if (substr(OPL[i], 1, 1) == "U") applyop(OPL[i])
		for (i = 1; i <= no; i++) if (substr(OPL[i], 1, 1) == "D") applyop(OPL[i])
	} else if (F == "D_before_U") {
		for (i = 1; i <= no; i++) if (substr(OPL[i], 1, 1) == "D") applyop(OPL[i])
		for (i = 1; i <= no; i++) if (substr(OPL[i], 1, 1) == "U") applyop(OPL[i])
	} else {
		for (i = 1; i <= no; i++) applyop(OPL[i])
	}
	if (nf == 0 && F != "noargs_empty") FILES[++nf] = "-"
	if (F == "files_reversed") {
		for (i = 1; i <= nf / 2; i++) { t = FILES[i]; FILES[i] = FILES[nf - i + 1]; FILES[nf - i + 1] = t }
	}
	if (F == "only_first_file") nf = 1
	if (F == "only_last_file") { FILES[1] = FILES[nf]; nf = 1 }
	if (F == "stdin_also") FILES[++nf] = "-"
	if (F == "dash_literal") { }
	for (fi = 1; fi <= nf; fi++) {
		if (F == "dash_skipped" && FILES[fi] == "-") continue
		if (F == "dash_literal" && FILES[fi] == "-") { warn("cannot open -"); continue }
		processfile(FILES[fi])
	}
	if (F == "m4wrap_reverse") {
		while (NW > 0) { IN = WRAP[NW]; NW--; run() }
	}
	while (WI < NW) {
		WI++
		IN = WRAP[WI]
		run()
	}
	finish(0)
}
function applyop(op,   k, v, p, nm, val) {
	k = substr(op, 1, 1); v = substr(op, 2)
	if (k == "U") {
		if (F == "U_ignored") return
		if (F == "U_first_only" && UDONE++ > 0) return
		if (F == "U_not_builtin" && ISBI[v, SP[v]]) return
		SP[v] = 0; return
	}
	if (F == "D_ignored") return
	p = index(v, "=")
	if (F == "D_split_last") { p = 0; for (q = 1; q <= length(v); q++) if (substr(v, q, 1) == "=") p = q }
	if (p == 0) { nm = v; val = ""; if (F == "D_noeq_one") val = "1" }
	else { nm = substr(v, 1, p - 1); val = substr(v, p + 1) }
	if (F == "D_noeq_undefined" && p == 0) return
	if (F == "D_empty_value_undefined" && p != 0 && val == "") return
	if (F == "D_value_first_word") sub(/ .*/, "", val)
	if (F == "D_value_ignored" && p != 0) val = ""
	setdef(nm, val, 0)
}

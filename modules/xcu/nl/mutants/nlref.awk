# The line-numbering loop of nlref.sh. Options arrive through the environment.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
function bre2ere(s,    i, c, nx, out, inb) {
	out = ""
	inb = 0
	for (i = 1; i <= length(s); i++) {
		c = substr(s, i, 1)
		if (inb) {
			out = out c
			if (c == "]" && i > 1) inb = 0
		} else if (c == "[") {
			out = out c
			inb = 1
			if (substr(s, i + 1, 1) == "^") { out = out "^"; i++ }
			if (substr(s, i + 1, 1) == "]") { out = out "]"; i++ }
		} else if (c == "\\") {
			nx = substr(s, i + 1, 1)
			i++
			if (nx ~ /[(){}]/) out = out nx
			else out = out "\\" nx
		} else if (c ~ /[+?|(){}]/) out = out "\\" c
		else out = out c
	}
	return out
}
function matches(st, line,    re, k) {
	re = substr(st, 2)
	if (M == "p_ere") k = (line ~ re)
	else if (M == "p_literal") k = (index(line, re) > 0)
	else if (M == "p_whole") k = (line ~ ("^(" bre2ere(re) ")$"))
	else if (M == "p_icase") k = (tolower(line) ~ tolower(bre2ere(re)))
	else k = (line ~ bre2ere(re))
	if (M == "p_inverted") k = !k
	if (M == "p_empty_skip" && line == "") k = 0
	return k
}
function wants(st, line,    k) {
	k = substr(st, 1, 1)
	if (k == "a") {
		if (line != "") { brun = 0; return 1 }
		brun++
		if (brun >= L) { brun = 0; return 1 }
		return 0
	}
	if (k == "t") return (M == "t_graph") ? (line ~ /[^ \t]/) : (line != "")
	if (k == "p") return matches(st, line)
	return 0
}
function fmtnum(n,    s) {
	if (fmt == "ln") s = sprintf("%-" W "d", n)
	else if (fmt == "rz") s = sprintf("%0" W "d", n)
	else s = sprintf("%" W "d", n)
	if (M == "ln_fixed_pad") s = n "     "
	return s
}
BEGIN {
	M = ENVIRON["PCN_M"]
	bt = ENVIRON["PCN_BT"]; ht = ENVIRON["PCN_HT"]; ft = ENVIRON["PCN_FT"]
	incr = ENVIRON["PCN_INCR"] + 0; L = ENVIRON["PCN_L"] + 0
	fmt = ENVIRON["PCN_FMT"]; sep = ENVIRON["PCN_SEP"]; start = ENVIRON["PCN_START"] + 0
	W = ENVIRON["PCN_W"] + 0
	d1 = ENVIRON["PCN_D1"]; d2 = ENVIRON["PCN_D2"]; pflag = ENVIRON["PCN_P"]
	n = start; sect = "b"; brun = 0; held = 0
	if (M == "i_first_step") first_step = 1
}
{
	line = $0
	if (M == "long_trunc") line = substr(line, 1, 255)
	dl = d1 d2
	hd = dl dl dl; bd = dl dl; fd = dl
	isd = 0
	if (M == "delim_prefix") {
		if (substr(line, 1, 6) == hd) { isd = 1; ns = "h" }
		else if (substr(line, 1, 4) == bd) { isd = 1; ns = "b" }
		else if (substr(line, 1, 2) == fd) { isd = 1; ns = "f" }
	} else if (line == hd) { isd = 1; ns = "h" }
	else if (line == bd) { isd = 1; ns = "b" }
	else if (line == fd) { isd = 1; ns = "f" }
	if (!isd && M == "d_adds_default") {
		if (line == "\\:\\:\\:") { isd = 1; ns = "h" }
		else if (line == "\\:\\:") { isd = 1; ns = "b" }
		else if (line == "\\:") { isd = 1; ns = "f" }
	}
	if (isd) {
		if (M == "hdr_delim_as_body" && ns == "h") ns = "b"
		if (M == "ftr_delim_ignored" && ns == "f") { isd = 0 }
		if (M == "body_delim_ignored" && ns == "b") { isd = 0 }
	}
	if (isd) {
		if (ns == "h") {
			if (M == "page_no_reset") { }
			else if (pflag != "" && M != "p_ignored") { }
			else n = (M == "reset_to_1") ? 1 : start
		}
		sect = ns
		if (M == "delim_dropped") next
		if (M == "delim_echo") print line
		else print ""
		next
	}
	st = (sect == "h") ? ht : (sect == "f") ? ft : bt
	if (M == "l_text") {
		cnt++
		ok = (cnt % L == 0)
	} else if (M == "l_first" && substr(st, 1, 1) == "a" && line == "") {
		brun++
		ok = (brun % L == 1 || L == 1)
	} else if (M == "l_global" && substr(st, 1, 1) == "a" && line == "") {
		grun++
		ok = (grun % L == 0)
	} else ok = wants(st, line)
	if (ok) {
		if (first_step) { n = n + incr; first_step = 0 }
		num = fmtnum(n)
		if (M == "text_first") out = line sep num
		else out = num sep line
		n += incr
		print out
	} else {
		if (M == "unnum_dropped") next
		if (M == "unnum_padded") print sprintf("%" W "s", "") sep line
		else if (M == "unnum_sep") print sep line
		else if (M == "unnum_spaces") print sprintf("%" W "s", "") line
		else print line
	}
}

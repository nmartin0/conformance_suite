# Regroups the expression of a find command line (one word per line) with
# explicit parentheses, so the real utility evaluates the grouping chosen
# by the mode. Words before the expression pass through.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
function arity(w) {
	if (w ~ /^-(name|iname|path|type|links|user|group|size|atime|ctime|mtime|perm|newer)$/) return 1
	return 0
}
function look() { return pos <= n ? t[pos] : "" }
function unary(   w, r, x) {
	w = look()
	if (w == "!") { pos++; x = unary(); return "!\n" x }
	if (w == "(") { pos++; r = top(); pos++; return "(\n" r "\n)" }
	pos++
	r = w
	if (w == "-exec" || w == "-ok") {
		while (pos <= n) {
			x = t[pos]; pos++
			r = r "\n" x
			if (x == ";" || (x == "+" && t[pos-2] == "{}")) break
		}
		return r
	}
	if (arity(w)) { r = r "\n" t[pos]; pos++ }
	return r
}
function andc(l, r) {
	if (mode == "noshort_and")
		return "(\n(\n" l "\n-a\n" r "\n)\n-o\n(\n!\n" l "\n-a\n(\n" r "\n-a\n!\n-links\n+0\n)\n)\n)"
	return "(\n" l "\n-a\n" r "\n)"
}
function orc(l, r) {
	if (mode == "noshort_or")
		return "(\n(\n" l "\n-a\n(\n" r "\n-o\n-links\n+0\n)\n)\n-o\n(\n!\n" l "\n-a\n" r "\n)\n)"
	return "(\n" l "\n-o\n" r "\n)"
}
# normal grammar: and binds tighter than or
function andx(   l, w) {
	l = unary()
	while (pos <= n && look() != "-o" && look() != ")") {
		if (look() == "-a") pos++
		l = andc(l, unary())
	}
	return l
}
function orx(   l) {
	l = andx()
	while (look() == "-o") { pos++; l = orc(l, andx()) }
	return l
}
# faulty grammar: or binds tighter than and
function orx2(   l) {
	l = unary()
	while (look() == "-o") { pos++; l = orc(l, unary()) }
	return l
}
function andx2(   l) {
	l = orx2()
	while (pos <= n && look() != ")") {
		if (look() == "-a") pos++
		l = andc(l, orx2())
	}
	return l
}
function top() { return (mode == "prec_or_tighter") ? andx2() : orx() }
{ w[NR] = $0 }
END {
	i = 1
	while (i <= NR && (w[i] == "-H" || w[i] == "-L" || (w[i] !~ /^-/ && w[i] != "!" && w[i] != "("))) { print w[i]; i++ }
	n = 0
	for (; i <= NR; i++) t[++n] = w[i]
	pos = 1
	if (n > 0) print top()
}

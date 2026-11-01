# -t drops header and trailer, a small -l does the same, -l sets the page length.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.pr.002
# MUTANT    pr.t_ignored
# MUTANT    pr.t_pads
# MUTANT    pr.l_ignored
# MUTANT    pr.l10_header
# MUTANT    pr.l11_suppressed
# MUTANT    pr.l_body_is_lines
# MUTANT    pr.l_plus1
# MUTANT    pr.trailer4
# MUTANT    pr.header4a
# MUTANT    pr.header4b
. "$PC_CORE/pc.sh"
pc_begin XCU.pr.002
LC_ALL=POSIX
export LC_ALL
NL='
'
mkn() { _n=$1; _f=$2; : >"$_f" || pc_unresolved "cannot create input"; _i=1; while [ "$_i" -le "$_n" ]; do printf 'L%s\n' "$_i"; _i=$((_i+1)); done >"$_f"; }
# pages <first> <last> <per-page>: pages of that many lines, each with a 5-line header and a 5-line trailer
pages() {
	pg_out=; pg_a=$1; pg_p=1
	while [ "$pg_a" -le "$2" ]; do
		pg_out=$pg_out"$NL${NL}Jan  2 03:04 2020 f Page $pg_p$NL$NL$NL"
		pg_b=$((pg_a+$3-1))
		while [ "$pg_a" -le "$pg_b" ]; do pg_out=$pg_out"L$pg_a$NL"; pg_a=$((pg_a+1)); done
		pg_out=$pg_out"$NL$NL$NL$NL$NL"
		pg_p=$((pg_p+1))
	done
}
mkn 30 f
TZ=UTC0 touch -t 202001020304 f || pc_unresolved "cannot set the modification time"
pc_test
got=$(pr -t f 2>/dev/null </dev/null; printf x)
[ "$got" = "$(cat f; printf x)" ] || pc_fail "-t did not give the input back unchanged"
got=$(pr -l 10 f 2>/dev/null </dev/null; printf x)
[ "$got" = "$(cat f; printf x)" ] || pc_fail "a page length not above the header plus trailer did not suppress them"
got=$(TZ=UTC0 pr -l 15 f 2>/dev/null </dev/null; printf x)
pages 1 30 5
[ "$got" = "$pg_out"x ] || pc_fail "-l 15 did not give six pages of a 5-line header, 5 lines and a 5-line trailer"
got=$(TZ=UTC0 pr -l 11 f 2>/dev/null </dev/null; printf x)
pages 1 30 1
[ "$got" = "$pg_out"x ] || pc_fail "-l 11 did not give one line between a 5-line header and a 5-line trailer on each page"
pc_pass

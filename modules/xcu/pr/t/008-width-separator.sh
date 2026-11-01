# Columns truncate long lines, -w sets the width, -s uses a separator and the 512 default width.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.pr.008
# MUTANT    pr.w_ignored
# MUTANT    pr.w_double
# MUTANT    pr.w_halved
# MUTANT    pr.default_w80
# MUTANT    pr.default_w40
# MUTANT    pr.s_default72
# MUTANT    pr.s_char_ignored
# MUTANT    pr.s_trailing
# MUTANT    pr.s_default_space
# MUTANT    pr.col_no_trunc
# MUTANT    pr.single_cut72
# MUTANT    pr.single_cut_w
# VARIANT   pr.pad_to_width
. "$PC_CORE/pc.sh"
pc_begin XCU.pr.008
LC_ALL=POSIX
export LC_ALL
NL='
'
TAB=$(printf '\t')
x30=xxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
x60=$x30$x30
x100=$x60$x30
printf '%s\nb\n' "$x100" >f || pc_unresolved "cannot create input"
printf '%s\nc\n' "$x60" >g || pc_unresolved "cannot create input"
printf 'a\n%s\n' "$x100" >h || pc_unresolved "cannot create input"
pc_test
got=$(pr -t -2 f 2>/dev/null </dev/null | head -n 1 | expand)
[ "${#got}" -le 72 ] || pc_fail "a line longer than a column was not truncated to the default width"
case $got in *x*) ;; *) pc_fail "the truncated line has lost its text";; esac
got=$(pr -t -2 h 2>/dev/null </dev/null | head -n 1 | expand)
[ "${#got}" -le 72 ] || pc_fail "a long second column was not truncated to the default width of 72"
x=$(printf '%s' "$got" | tr -cd x | wc -c)
[ "$x" -ge 25 ] || pc_fail "the second column kept only $x characters of its text with the default width"
got=$(pr -t -2 -w 20 h 2>/dev/null </dev/null | head -n 1 | expand)
[ "${#got}" -le 20 ] || pc_fail "-w 20 did not limit the line to 20 columns"
case $got in *x*) ;; *) pc_fail "the -w 20 line has lost its text";; esac
got=$(pr -t -2 -w 200 g 2>/dev/null </dev/null | head -n 1)
case $got in *"$x60"*c*) ;; *) pc_fail "-w 200 truncated a 60-column text";; esac
got=$(pr -t -2 -s: g 2>/dev/null </dev/null)
[ "$got" = "${x60}:c" ] || pc_fail "-s: did not separate the columns with a colon and keep the 60-column text"
got=$(pr -t -2 -s g 2>/dev/null </dev/null)
[ "$got" = "${x60}${TAB}c" ] || pc_fail "-s without a character did not separate the columns with a tab"
got=$(pr -t f 2>/dev/null </dev/null | head -n 1)
[ "$got" = "$x100" ] || pc_fail "single-column output was truncated"
got=$(pr -t -w 20 f 2>/dev/null </dev/null | head -n 1)
[ "$got" = "$x100" ] || pc_fail "-w truncated single-column output"
pc_pass

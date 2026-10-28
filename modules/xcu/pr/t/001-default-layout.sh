# pr paginates into 66-line pages with a 5-line header and 5-line trailer.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.pr.001
. "$PC_CORE/pc.sh"
pc_begin XCU.pr.001
LC_ALL=POSIX
export LC_ALL
NL='
'
mkn() { _n=$1; _f=$2; : >"$_f" || pc_unresolved "cannot create input"; _i=1; while [ "$_i" -le "$_n" ]; do printf 'L%s\n' "$_i"; _i=$((_i+1)); done >"$_f"; }
rng() { _a=$1; _b=$2; rng_out=; while [ "$_a" -le "$_b" ]; do rng_out=$rng_out"L$_a$NL"; _a=$((_a+1)); done; }
mkn 100 f
touch -t 202001020304 f || pc_unresolved "cannot set the modification time"
hd() { printf '\n\n%s Page %s\n\n\n' "$1" "$2"; }
pc_test
got=$(TZ=UTC0 pr f 2>/dev/null </dev/null; printf 'x')
rng 1 56; p1=$(hd "Jan  2 03:04 2020 f" 1; printf x); p1=${p1%x}$rng_out$NL$NL$NL$NL$NL
case $got in "$p1"*) ;; *) pc_fail "the first page is not a 5-line header, 56 lines and a 5-line trailer";; esac
rest=${got#"$p1"}
rng 57 100; p2=$(hd "Jan  2 03:04 2020 f" 2; printf x); p2=${p2%x}$rng_out
case $rest in "$p2"*) ;; *) pc_fail "the second page does not start with its header and the next lines";; esac
rem=${rest#"$p2"}; rem=${rem%x}
case $rem in *[!$NL]*) pc_fail "text follows the last line other than blank lines";; esac
[ "${#rem}" -ge 5 ] || pc_fail "the last page has no 5-line trailer"
pc_pass

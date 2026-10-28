# diff -c and -C n write a header with the file names and times, then hunks of changed lines (marked !) with the requested number of context lines, once for each file.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.diff.003
. "$PC_CORE/pc.sh"
pc_begin XCU.diff.003
LC_ALL=POSIX
export LC_ALL
NL='
'
mkf() {
	n=$1
	shift
	: >"$n" || pc_unresolved "cannot create input"
	for l in "$@"; do printf '%s\n' "$l" >>"$n"; done
}
TZ=UTC0
export TZ
mkf f1 a b c d e f g
mkf f2 a b c X e f g
touch -t 202001020304.05 f1 || pc_unresolved "cannot set the time of f1"
touch -t 202103040506.07 f2 || pc_unresolved "cannot set the time of f2"
pc_test
body() { # drop the two header lines of the output in $1
	b=${1#*"$NL"}
	b=${b#*"$NL"}
	printf '%s' "$b"
}
out=$(diff -c f1 f2 2>/dev/null </dev/null)
case $out in
"*** f1 Thu Jan  2 03:04:05 2020${NL}--- f2 Thu Mar  4 05:06:07 2021${NL}"*) ;;
*) pc_fail "the -c header lines are not '*** f1 <date>' and '--- f2 <date>' with the file times" ;;
esac
want="***************${NL}*** 1,7 ****${NL}  a${NL}  b${NL}  c${NL}! d${NL}  e${NL}  f${NL}  g${NL}--- 1,7 ----${NL}  a${NL}  b${NL}  c${NL}! X${NL}  e${NL}  f${NL}  g"
[ "$(body "$out")" = "$want" ] || pc_fail "-c did not give three lines of context with ! marks"
out=$(diff -C 1 f1 f2 2>/dev/null </dev/null)
want="***************${NL}*** 3,5 ****${NL}  c${NL}! d${NL}  e${NL}--- 3,5 ----${NL}  c${NL}! X${NL}  e"
[ "$(body "$out")" = "$want" ] || pc_fail "-C 1 did not give one line of context"
out=$(diff -C 2 f1 f2 2>/dev/null </dev/null)
want="***************${NL}*** 2,6 ****${NL}  b${NL}  c${NL}! d${NL}  e${NL}  f${NL}--- 2,6 ----${NL}  b${NL}  c${NL}! X${NL}  e${NL}  f"
[ "$(body "$out")" = "$want" ] || pc_fail "-C 2 did not give two lines of context"
diff -c f1 f2 >/dev/null 2>&1 </dev/null
[ $? -eq 1 ] || pc_fail "-c with differences did not give status 1"
out=$(diff -c f1 f1 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$out" = "rc=0" ] || pc_fail "-c with identical files wrote output or gave a status other than 0"
mkf g1 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20
mkf g2 1 X 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 Y 20
out=$(diff -C 1 g1 g2 2>/dev/null </dev/null)
want="***************${NL}*** 1,3 ****${NL}  1${NL}! 2${NL}  3${NL}--- 1,3 ----${NL}  1${NL}! X${NL}  3${NL}***************${NL}*** 18,20 ****${NL}  18${NL}! 19${NL}  20${NL}--- 18,20 ----${NL}  18${NL}! Y${NL}  20"
[ "$(body "$out")" = "$want" ] || pc_fail "two distant changes did not give two separate hunks"
pc_pass

# diff -u and -U n write a header with the file names and times, then @@ hunks with context, deleted (-) and added (+) lines interleaved in file order.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.diff.004
. "$PC_CORE/pc.sh"
pc_begin XCU.diff.004
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
tab=$(printf '\t')
mkf f1 a b c d e f g
mkf f2 a b c X e f g
touch -t 202001020304.05 f1 || pc_unresolved "cannot set the time of f1"
touch -t 202103040506.07 f2 || pc_unresolved "cannot set the time of f2"
pc_test
body() {
	b=${1#*"$NL"}
	b=${b#*"$NL"}
	printf '%s' "$b"
}
frac_ok() { # $1 is what follows the seconds field, up to the zone
	case $1 in
	'') return 0 ;;
	.[0-9]*)
		case ${1#.} in
		*[!0-9]*) return 1 ;;
		esac
		return 0
		;;
	esac
	return 1
}
out=$(diff -u f1 f2 2>/dev/null </dev/null)
h1=${out%%"$NL"*}
rest=${out#*"$NL"}
h2=${rest%%"$NL"*}
case $h1 in
"--- f1${tab}2020-01-02 03:04:05"*" +0000") ;;
*) pc_fail "the first -u header line is not '--- f1<tab>2020-01-02 03:04:05[.frac] +0000'" ;;
esac
f=${h1#"--- f1${tab}2020-01-02 03:04:05"}
f=${f% +0000}
frac_ok "$f" || pc_fail "the fraction in the first -u header line is not empty or a point and digits"
case $h2 in
"+++ f2${tab}2021-03-04 05:06:07"*" +0000") ;;
*) pc_fail "the second -u header line is not '+++ f2<tab>2021-03-04 05:06:07[.frac] +0000'" ;;
esac
want="@@ -1,7 +1,7 @@${NL} a${NL} b${NL} c${NL}-d${NL}+X${NL} e${NL} f${NL} g"
[ "$(body "$out")" = "$want" ] || pc_fail "-u did not give three lines of context with - and + lines"
out=$(diff -U 1 f1 f2 2>/dev/null </dev/null)
want="@@ -3,3 +3,3 @@${NL} c${NL}-d${NL}+X${NL} e"
[ "$(body "$out")" = "$want" ] || pc_fail "-U 1 did not give one line of context"
out=$(diff -U 2 f1 f2 2>/dev/null </dev/null)
want="@@ -2,5 +2,5 @@${NL} b${NL} c${NL}-d${NL}+X${NL} e${NL} f"
[ "$(body "$out")" = "$want" ] || pc_fail "-U 2 did not give two lines of context"
mkf h1 1 2 3 4 5
mkf h2 1 X Y 4 5
out=$(diff -U 0 h1 h2 2>/dev/null </dev/null)
want="@@ -2,2 +2,2 @@${NL}-2${NL}-3${NL}+X${NL}+Y"
[ "$(body "$out")" = "$want" ] || pc_fail "-U 0 did not give only the changed lines"
mkf i1 a b c
mkf i2 a b x c
out=$(diff -U 1 i1 i2 2>/dev/null </dev/null)
want="@@ -2,2 +2,3 @@${NL} b${NL}+x${NL} c"
[ "$(body "$out")" = "$want" ] || pc_fail "an added line in -U 1 wrong"
mkf j1 a b c
mkf j2 a c
out=$(diff -U 1 j1 j2 2>/dev/null </dev/null)
want="@@ -1,3 +1,2 @@${NL} a${NL}-b${NL} c"
[ "$(body "$out")" = "$want" ] || pc_fail "a deleted line in -U 1 wrong"
mkf k1 1 2 3 4 5 6 7 8 9 10
mkf k2 1 X 3 4 5 6 7 8 Y 10
out=$(diff -U 1 k1 k2 2>/dev/null </dev/null)
want="@@ -1,3 +1,3 @@${NL} 1${NL}-2${NL}+X${NL} 3${NL}@@ -8,3 +8,3 @@${NL} 8${NL}-9${NL}+Y${NL} 10"
[ "$(body "$out")" = "$want" ] || pc_fail "distant changes did not give two hunks"
mkf l1 1 2 3 4 5
mkf l2 1 X 3 Y 5
out=$(diff -U 1 l1 l2 2>/dev/null </dev/null)
want="@@ -1,5 +1,5 @@${NL} 1${NL}-2${NL}+X${NL} 3${NL}-4${NL}+Y${NL} 5"
[ "$(body "$out")" = "$want" ] || pc_fail "changes two lines apart were not joined into one hunk"
diff -u f1 f2 >/dev/null 2>&1 </dev/null
[ $? -eq 1 ] || pc_fail "-u with differences did not give status 1"
pc_pass

# A pair of identical items only records the item, and blanks and newlines in any number separate items.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.tsort.002
. "$PC_CORE/pc.sh"
pc_begin XCU.tsort.002
LC_ALL=POSIX
export LC_ALL
NL='
'
tcheck() {
	why=
	for v in a b c d e f g h x y z; do unset "p_$v" "s_$v"; done
	printf '%b' "$1" >ts.in || { why="cannot write input"; return 1; }
	tsort <ts.in >ts.out 2>/dev/null
	rc=$?
	[ $rc -eq 0 ] || { why="status $rc"; return 1; }
	n=0
	while IFS= read -r l; do
		case $l in
		'' | *[!A-Za-z0-9]*) why="odd output line '$l'"; return 1 ;;
		esac
		eval "[ -z \"\${p_$l-}\" ]" || { why="item $l twice"; return 1; }
		n=$((n + 1))
		eval "p_$l=$n"
	done <ts.out
	set -- $(cat ts.in)
	d=0
	while [ $# -ge 2 ]; do
		x=$1
		y=$2
		shift 2
		for it in $x $y; do
			eval "[ -n \"\${p_$it-}\" ]" || { why="item $it missing"; return 1; }
			eval "[ -z \"\${s_$it-}\" ]" && { eval "s_$it=1"; d=$((d + 1)); }
		done
		if [ "$x" != "$y" ]; then
			eval "[ \$p_$x -lt \$p_$y ]" || { why="$x not before $y"; return 1; }
		fi
	done
	[ $n -eq $d ] || { why="$n lines for $d items"; return 1; }
	return 0
}
pc_test
got=$(printf 'a a\n' | tsort 2>/dev/null; printf 'rc=%s' "$?")
[ "$got" = "a${NL}rc=0" ] || pc_fail "a pair of identical items did not give just that item"
tcheck 'x x\ny y\n' || pc_fail "two identical pairs: $why"
tcheck 'x y\ny y\nz z\n' || pc_fail "identical pairs mixed with ordering: $why"
got=$(printf 'a   b\n\n\nb\nc\n' | tsort 2>/dev/null)
[ "$got" = "a${NL}b${NL}c" ] || pc_fail "runs of blanks and blank lines did not separate items"
got=$(printf 'a b b c\n' | tsort 2>/dev/null)
[ "$got" = "a${NL}b${NL}c" ] || pc_fail "items on one line wrong"
got=$(printf 'a\nb\nb\nc\n' | tsort 2>/dev/null)
[ "$got" = "a${NL}b${NL}c" ] || pc_fail "items one per line wrong"
got=$(printf 'a b a b a b\n' | tsort 2>/dev/null)
[ "$got" = "a${NL}b" ] || pc_fail "a repeated pair gave a repeated item"
got=$(printf 'long1 long2\nlong2 3\n' | tsort 2>/dev/null)
[ "$got" = "long1${NL}long2${NL}3" ] || pc_fail "multi-character items wrong"
pc_pass

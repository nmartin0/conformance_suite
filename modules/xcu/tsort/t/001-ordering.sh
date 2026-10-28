# tsort writes every item once, one per line, in an order that puts the first item of each pair of different items before the second.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.tsort.001
. "$PC_CORE/pc.sh"
pc_begin XCU.tsort.001
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
tcheck 'a b\nb c\nc d\n' || pc_fail "chain a b c d: $why"
got=$(tsort <ts.in 2>/dev/null)
[ "$got" = "a${NL}b${NL}c${NL}d" ] || pc_fail "the only valid order of a chain was not written"
tcheck 'a b c c d e\ng g\nf g e f\nh h\n' || pc_fail "page example: $why"
tcheck 'e f\nd e\nc d\nb c\na b\n' || pc_fail "chain given in reverse: $why"
got=$(tsort <ts.in 2>/dev/null)
[ "$got" = "a${NL}b${NL}c${NL}d${NL}e${NL}f" ] || pc_fail "the only valid order of a reversed chain was not written"
tcheck 'a b a c a d\n' || pc_fail "one item before three: $why"
tcheck 'b a c a d a\n' || pc_fail "three items before one: $why"
tcheck 'a b a c b d c d\n' || pc_fail "diamond: $why"
tcheck 'a b\nc d\n' || pc_fail "two separate chains: $why"
pc_pass

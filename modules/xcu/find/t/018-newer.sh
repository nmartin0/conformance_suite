# -newer file matches files modified more recently than file.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.find.018
. "$PC_CORE/pc.sh"
pc_begin XCU.find.018
# same_set GOT WANT: the words of GOT are exactly the words of WANT, each
# once, in any order (shell only, so no other utility is involved).
same_set() {
	s_a=" " s_b=" " s_n=0 s_m=0
	for s_w in $1; do s_a="$s_a$s_w "; s_n=$((s_n + 1)); done
	for s_w in $2; do s_b="$s_b$s_w "; s_m=$((s_m + 1)); done
	[ "$s_n" -eq "$s_m" ] || return 1
	for s_w in $1; do
		case $s_b in
		*" $s_w "*) ;;
		*) return 1 ;;
		esac
	done
	for s_w in $2; do
		case $s_a in
		*" $s_w "*) ;;
		*) return 1 ;;
		esac
	done
	return 0
}

mkdir n || pc_fail "cannot create n"
touch -t 200501010101 n/ref || pc_fail "cannot create n/ref"
touch -t 200101010101 n/o || pc_fail "cannot create n/o"
touch -t 201001010101 n/w n/x || pc_fail "cannot create n/w"
pc_test
got=$(find n -type f -newer n/ref 2>err </dev/null)
rc=$?
[ "$rc" -eq 0 ] || pc_fail "-newer: find exited with status $rc"
same_set "$got" "n/w n/x" || pc_fail "-newer: found '$got'"
pc_pass

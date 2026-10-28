# -newer file matches files modified more recently than file.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.find.018
. "$PC_CORE/pc.sh"
pc_begin XCU.find.018
same_set() {
	s_a=$(printf '%s\n' $1 | sort)
	s_b=$(printf '%s\n' $2 | sort)
	[ "$s_a" = "$s_b" ]
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

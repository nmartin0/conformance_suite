# -links n matches files with exactly, more than (+n) or fewer than (-n) n links.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.find.009
# MUTANT   find.links_plus_ge
# MUTANT   find.links_minus_le
# MUTANT   find.links_signs_swapped
# MUTANT   find.links_exact_ge
# MUTANT   find.links_exact_le
# MUTANT   find.links_sign_ignored
. "$PC_CORE/pc.sh"
pc_begin XCU.find.009
same_set() {
	s_a=$(printf '%s\n' $1 | sort)
	s_b=$(printf '%s\n' $2 | sort)
	[ "$s_a" = "$s_b" ]
}

mkdir t t/sub t/empty || pc_fail "cannot create directories"
printf 'a\n' >t/a.txt || pc_fail "cannot create t/a.txt"
printf 'bb\n' >t/B.TXT || pc_fail "cannot create t/B.TXT"
printf 'c\n' >t/sub/c.txt || pc_fail "cannot create t/sub/c.txt"
printf 'd\n' >t/sub/d.c || pc_fail "cannot create t/sub/d.c"
: >t/.hid || pc_fail "cannot create t/.hid"
ln -s a.txt t/l || pc_fail "cannot create symbolic link"
mkfifo t/p || pc_fail "cannot create FIFO"
ln t/a.txt t/hard || pc_fail "cannot create hard link"
ln t/a.txt t/hard2 || pc_fail "cannot create hard link"
ln t/sub/c.txt t/c2 || pc_fail "cannot create hard link"
pc_test
for lk in "2|t/sub/c.txt t/c2" "3|t/a.txt t/hard t/hard2" "+2|t/a.txt t/hard t/hard2" "+1|t/a.txt t/hard t/hard2 t/sub/c.txt t/c2" "-3|t/sub/c.txt t/c2 t/B.TXT t/sub/d.c t/.hid" "-2|t/B.TXT t/sub/d.c t/.hid"; do
	arg=${lk%%|*}
	want=${lk#*|}
	got=$(find t -type f -links "$arg" 2>err </dev/null)
	rc=$?
	[ "$rc" -eq 0 ] || pc_fail "-links $arg: find exited with status $rc"
	same_set "$got" "$want" || pc_fail "-links $arg: found '$got'"
done
pc_pass

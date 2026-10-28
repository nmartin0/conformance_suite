# ls -C and -x with COLUMNS: down or across, whole names, constant columns
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ls.038
. "$PC_CORE/pc.sh"
pc_begin XCU.ls.038
LC_ALL=POSIX; export LC_ALL
mkdir d || pc_unresolved "cannot create d"
names='a bb ccc dddd eeeee ffffff g hh iii jjjj kkkkk llllll'
for n in $names; do : >"d/$n" || pc_unresolved "cannot create d/$n"; done
want=$(printf '%s\n' $names)
pc_test
for o in -C -x; do
	COLUMNS=40 ls $o d >raw 2>err </dev/null
	rc=$?
	[ "$rc" -eq 0 ] || pc_fail "ls $o exited with status $rc"
	expand <raw >out
	if [ "$o" = -C ]; then
		got=$(awk '{ for (i = 1; i <= NF; i++) t[NR, i] = $i; if (NF > mx) mx = NF } END { for (j = 1; j <= mx; j++) for (r = 1; r <= NR; r++) if ((r, j) in t) print t[r, j] }' out)
		how="down the columns"
	else
		got=$(awk '{ for (i = 1; i <= NF; i++) print $i }' out)
		how="across the rows"
	fi
	[ "$got" = "$want" ] || pc_fail "ls $o: reading $how gives '$(echo $got)', expected the collating order"
	longest=$(awk '{ if (length($0) > m) m = length($0) } END { print m + 0 }' out)
	[ "$longest" -le 40 ] || pc_fail "ls $o with COLUMNS=40 wrote a line of $longest columns"
	awk '{
		pos = 1
		for (k = 1; k <= NF; k++) {
			s = index(substr($0, pos), $k)
			start = pos + s - 1
			pos = start + length($k)
			if (k in st) { if (st[k] != start) bad = 1 } else st[k] = start
		}
	} END { exit bad + 0 }' out || pc_fail "ls $o: the names of a column do not start at the same position in every line"
done
pc_pass

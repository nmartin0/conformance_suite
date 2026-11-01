# A -f prefix that would make a file name longer than the largest the directory allows is an error and no file is created.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.csplit.011
# MUTANT    csplit.long_truncate
# MUTANT    csplit.long_creates
# MUTANT    csplit.long_status0
# MUTANT    csplit.long_no_diag
# MUTANT    csplit.long_diag_stdout
. "$PC_CORE/pc.sh"
pc_begin XCU.csplit.011
LC_ALL=POSIX
export LC_ALL
printf '%s\n' 1 2 3 4 5 6 7 8 9 10 >in10 || pc_fail "cannot create input"
mkdir work || pc_fail "cannot make a directory"
max=$(getconf NAME_MAX work 2>/dev/null) || pc_notinuse "no NAME_MAX for the directory"
case $max in
'' | *[!0-9]*) pc_notinuse "NAME_MAX has no numeric limit here" ;;
esac
[ "$max" -ge 14 ] && [ "$max" -le 1000 ] || pc_notinuse "NAME_MAX of $max is not tried"
# a prefix of NAME_MAX-1 bytes: with two digits the name has NAME_MAX+1 bytes
pre=a
while [ "${#pre}" -lt "$max" ]; do pre=$pre$pre; done
pre=$(printf '%s' "$pre" | cut -c1-$((max - 1)))
[ "${#pre}" -eq $((max - 1)) ] || pc_unresolved "cannot build the prefix"
pc_test
cd work || pc_fail "cannot enter the directory"
csplit -f "$pre" ../in10 5 >out 2>err </dev/null
rc=$?
[ "$rc" -ne 0 ] || pc_fail "status 0 for a file name longer than NAME_MAX"
[ -s err ] || pc_fail "no diagnostic on standard error"
for f in * .[!.]* ..?*; do
	case $f in out | err) continue ;; esac
	[ -e "$f" ] && pc_fail "a file was created: $f"
done
cd ..
pc_pass

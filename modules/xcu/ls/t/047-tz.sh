# ls -l: the time of a recent file follows TZ
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ls.047
. "$PC_CORE/pc.sh"
pc_begin XCU.ls.047
LC_ALL=POSIX; export LC_ALL
: >f || pc_unresolved "cannot create f"
# hm <TZ>: hour and minute of the time written by ls -l
hm() {
	TZ=$1 ls -l f 2>/dev/null | awk '{ print $8 }'
}
a=$(hm UTC0)
case $a in
[0-9][0-9]:[0-9][0-9]) ;;
*) pc_unresolved "ls -l wrote '$a' for a file just created, not an hour and minute" ;;
esac
pc_test
b=$(hm XYZ-3)
case $b in
[0-9][0-9]:[0-9][0-9]) ;;
*) pc_fail "with TZ=XYZ-3 the time field is '$b'" ;;
esac
ha=${a%:*}; ma=${a#*:}; hb=${b%:*}; mb=${b#*:}
[ "$ma" = "$mb" ] || pc_fail "the minutes differ between the two zones: $a and $b"
d=$(( (${hb#0} - ${ha#0} + 24) % 24 ))
[ "$d" -eq 3 ] || pc_fail "TZ=XYZ-3 writes $b, which is $d hours after $a (UTC0), expected 3"
pc_pass

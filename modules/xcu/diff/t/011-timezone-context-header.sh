# The timestamps that diff -c writes are the modification times of the files in the zone that TZ names.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.diff.011
. "$PC_CORE/pc.sh"
pc_begin XCU.diff.011
LC_ALL=POSIX
export LC_ALL
pcl_d=pcl.$$
mkdir "$pcl_d" || pc_unresolved "cannot make a directory"
cd "$pcl_d" || pc_unresolved "cannot enter the directory"
trap 'cd ..; rm -rf "$pcl_d"' EXIT
printf 'a\n' >f || pc_unresolved "cannot write a file"
printf 'b\n' >g || pc_unresolved "cannot write a file"
TZ=PCA0 touch -t 202106151030 f g || pc_unresolved "cannot set the modification time"
pc_test
for pcl_z in PCA0:10 PCB-5:15 PCC5:05
do
	pcl_tz=${pcl_z%:*}
	pcl_h=${pcl_z#*:}
	pcl_line=$(TZ=$pcl_tz diff -c f g 2>/dev/null | sed -n 1p)
	set -- $pcl_line
	[ $# -eq 7 ] || pc_fail "with TZ=$pcl_tz the first header line was [$pcl_line]"
	[ "$4 $5 $6 $7" = "Jun 15 $pcl_h:30:00 2021" ] || pc_fail "with TZ=$pcl_tz the timestamp was [$4 $5 $6 $7], not [Jun 15 $pcl_h:30:00 2021]"
done
pc_pass

# The time in the page header of pr is the modification time of the file in the zone that TZ names.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.pr.013
. "$PC_CORE/pc.sh"
pc_begin XCU.pr.013
LC_ALL=POSIX
export LC_ALL
pcl_d=pcl.$$
mkdir "$pcl_d" || pc_unresolved "cannot make a directory"
cd "$pcl_d" || pc_unresolved "cannot enter the directory"
trap 'cd ..; rm -rf "$pcl_d"' EXIT
printf 'x\n' >f || pc_unresolved "cannot write a file"
TZ=PCA0 touch -t 202106151030 f || pc_unresolved "cannot set the modification time"
pc_test
for pcl_z in PCA0:10 PCB-5:15 PCC5:05
do
	pcl_tz=${pcl_z%:*}
	pcl_h=${pcl_z#*:}
	pcl_line=$(TZ=$pcl_tz pr f 2>/dev/null | sed -n 3p)
	[ "$pcl_line" = "Jun 15 $pcl_h:30 2021 f Page 1" ] || pc_fail "with TZ=$pcl_tz the header line was [$pcl_line], not [Jun 15 $pcl_h:30 2021 f Page 1]"
done
pc_pass

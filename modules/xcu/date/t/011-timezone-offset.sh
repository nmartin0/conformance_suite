# date writes the time of the zone that TZ names, an offset ahead of or behind the zone with offset zero.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.date.011
. "$PC_CORE/pc.sh"
pc_begin XCU.date.011
LC_ALL=POSIX
export LC_ALL
pc_test
pcl_n=0
while :
do
	pcl_a=$(TZ=PCA0 date '+%H:%M')
	pcl_b=$(TZ=PCB-5 date '+%H:%M')
	pcl_c=$(TZ=PCC5 date '+%H:%M')
	pcl_e=$(TZ=PCA0 date '+%H:%M')
	[ "$pcl_a" = "$pcl_e" ] && break
	pcl_n=$((pcl_n + 1))
	[ "$pcl_n" -lt 5 ] || pc_unresolved "the clock changed minute during every attempt"
done
pcl_ha=${pcl_a%:*}
pcl_hb=${pcl_b%:*}
pcl_hc=${pcl_c%:*}
pcl_ha=${pcl_ha#0}
pcl_hb=${pcl_hb#0}
pcl_hc=${pcl_hc#0}
[ "${pcl_a#*:}" = "${pcl_b#*:}" ] && [ "${pcl_a#*:}" = "${pcl_c#*:}" ] || pc_fail "the minutes differ between zones (a=$pcl_a b=$pcl_b c=$pcl_c)"
pcl_xb=$(( ( ${pcl_ha:-0} + 5 ) % 24 ))
pcl_xc=$(( ( ${pcl_ha:-0} + 19 ) % 24 ))
[ "${pcl_hb:-0}" -eq "$pcl_xb" ] || pc_fail "TZ=PCB-5 (5 hours ahead) wrote $pcl_b when the zone with offset zero wrote $pcl_a"
[ "${pcl_hc:-0}" -eq "$pcl_xc" ] || pc_fail "TZ=PCC5 (5 hours behind) wrote $pcl_c when the zone with offset zero wrote $pcl_a"
pc_pass

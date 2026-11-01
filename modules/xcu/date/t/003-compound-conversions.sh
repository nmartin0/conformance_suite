# The compound conversions equal their parts (one invocation, one instant).
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.date.003
# MUTANT    date.D_year4
# MUTANT    date.T_no_seconds
# MUTANT    date.R_seconds
# MUTANT    date.F_slash
# MUTANT    date.h_full
# MUTANT    date.C_wrong
# MUTANT    date.r_24h
# MUTANT    date.r_no_meridiem
# MUTANT    date.x_year4
# MUTANT    date.X_no_seconds
# MUTANT    date.c_zone
# MUTANT    date.c_no_seconds
# VARIANT   date.leap_second
. "$PC_CORE/pc.sh"
pc_begin XCU.date.003
LC_ALL=C; export LC_ALL
TZ=UTC0; export TZ
pc_test
out=$(date '+%D|%m/%d/%y|%T|%H:%M:%S|%R|%H:%M|%F|%Y-%m-%d|%h|%b|%C%y|%Y|%r|%I:%M:%S %p|%x|%m/%d/%y|%X|%H:%M:%S|%c|%a %b %e %H:%M:%S %Y')
set -f
IFS='|'
set -- $out
IFS=' 	
'
set +f
[ "$#" -eq 20 ] || pc_fail "expected 20 fields, got $# from '$out'"
i=1
while [ "$#" -ge 2 ]; do
	[ -n "$1" ] || pc_fail "pair $i is empty"
	[ "$1" = "$2" ] || pc_fail "pair $i: '$1' differs from '$2'"
	shift 2
	i=$((i + 1))
done
pc_pass

# a conforming ar whose long listing leaves day, hour and minute unpadded
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# Allowed because the format string prints these fields with a plain number
# conversion (XCU ar STDOUT), so a leading zero or blank is not required.
ar() {
	case ${1:-} in
	-tv)
		pc_t=pc-var.$$
		command ar "$@" >"$pc_t"
		pc_rc=$?
		awk '{ split($6, a, ":"); $5 = $5 + 0; $6 = (a[1] + 0) ":" (a[2] + 0); print }' <"$pc_t"
		rm -f "$pc_t"
		return $pc_rc ;;
	esac
	command ar "$@"
}

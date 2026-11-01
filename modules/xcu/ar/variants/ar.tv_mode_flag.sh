# a conforming ar whose long listing adds an access-method flag to the mode
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# Allowed because the mode string is formatted as the ls file mode, which may
# end with one printable non-blank character (XCU ar STDOUT, XCU ls STDOUT).
ar() {
	case ${1:-} in
	-tv)
		pc_t=pc-var.$$
		command ar "$@" >"$pc_t"
		pc_rc=$?
		awk '{ $1 = $1 "+"; print }' <"$pc_t"
		rm -f "$pc_t"
		return $pc_rc ;;
	esac
	command ar "$@"
}

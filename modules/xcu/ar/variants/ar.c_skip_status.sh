# a conforming ar that exits 1 when -C kept an existing file
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# Allowed because the text does not say what the status is when an extracted
# file is not written because -C was given (XCU ar OPTIONS -C).
ar() {
	pc_c=0
	case ${1:-} in
	-x*C*) pc_c=1 ;;
	-x) [ "${2:-}" = -C ] && pc_c=1 ;;
	esac
	pc_ex=0
	if [ "$pc_c" -eq 1 ]; then
		pc_seen=0
		for pc_w in "$@"; do
			case $pc_w in
			-*) ;;
			*) if [ "$pc_seen" -eq 0 ]; then pc_seen=1; elif [ -e "${pc_w##*/}" ]; then pc_ex=1; fi ;;
			esac
		done
	fi
	command ar "$@"
	pc_rc=$?
	if [ "$pc_rc" -ne 0 ]; then return "$pc_rc"; fi
	if [ "$pc_ex" -eq 1 ]; then return 1; fi
	return 0
}

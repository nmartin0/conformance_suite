# a conforming sort that gives up on the first file it cannot read, before writing anything
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# Allowed: on an error opening a file operand sort may stop without writing
# output or processing later operands (XCU sort OPERANDS, CONSEQUENCES OF ERRORS).
sort() {
	for pc_a in "$@"; do
		case $pc_a in
		-*) ;;
		*) if [ ! -r "$pc_a" ]; then echo "sort: $pc_a: cannot open" >&2; return 2; fi ;;
		esac
	done
	command sort "$@"
}

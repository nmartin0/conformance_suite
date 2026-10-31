# a conforming sort -c whose diagnostic names a line number instead of quoting the line
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# Allowed: either kind of message is accepted (XCU sort RATIONALE on -c).
sort() {
	pc_c=
	for pc_a in "$@"; do
		case $pc_a in -*c*) pc_c=1 ;; esac
	done
	command sort "$@" 2>/dev/null
	pc_rc=$?
	if [ "$pc_rc" -eq 1 ] && [ -n "$pc_c" ]; then echo "sort: disorder found at line 2" >&2; fi
	return "$pc_rc"
}

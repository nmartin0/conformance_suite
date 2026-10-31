# a conforming sort whose error status is 255
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# Allowed: any status greater than 1 reports an error (XCU sort EXIT STATUS).
sort() {
	command sort "$@"
	pc_rc=$?
	if [ "$pc_rc" -gt 1 ]; then return 255; fi
	return "$pc_rc"
}

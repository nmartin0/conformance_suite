# conforming nl whose error status is 2
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# Allowed because the text asks only for a status greater than zero when an
# error occurred (XCU nl EXIT STATUS).
nl() {
	command nl "$@"
	pc_rc=$?
	if [ "$pc_rc" -ne 0 ]; then return 2; fi
	return 0
}

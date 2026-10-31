# a conforming cut whose error status is 2 rather than 1
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# Allowed because the text asks only for a status greater than zero when an
# error occurred (XCU cut EXIT STATUS).
cut() {
	command cut "$@"
	pc_rc=$?
	if [ "$pc_rc" -ne 0 ]; then return 2; fi
	return 0
}

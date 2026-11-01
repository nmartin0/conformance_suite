# a conforming mkdir whose error status is 255 rather than 1
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# Allowed because the text asks only for a status greater than zero when an
# error occurred (XCU mkdir EXIT STATUS).
mkdir() {
	command mkdir "$@"
	pc_rc=$?
	if [ "$pc_rc" -ne 0 ]; then return 255; fi
	return 0
}

# a conforming chmod whose error status is 255 rather than 1
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# Allowed because the text asks only for a status greater than zero (XCU chmod EXIT STATUS).
chmod() {
	command chmod "$@"
	pc_rc=$?
	if [ "$pc_rc" -ne 0 ]; then return 255; fi
	return 0
}

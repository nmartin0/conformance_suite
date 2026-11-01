# a conforming mv whose error status is 2
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# Allowed because the text asks only for a status greater than zero when an error occurred (XCU mv EXIT STATUS).
mv() {
	command mv "$@"
	pc_rc=$?
	if [ "$pc_rc" -ne 0 ]; then return 2; fi
	return 0
}

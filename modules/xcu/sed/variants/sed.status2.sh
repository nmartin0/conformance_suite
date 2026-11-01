# a conforming sed whose error status is 2 rather than 1
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# Allowed because the page asks only for a status above zero on an error (EXIT STATUS).
sed() {
	command sed "$@"
	pc_rc=$?
	if [ "$pc_rc" -ne 0 ]; then return 2; fi
	return 0
}

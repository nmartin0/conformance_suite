# a conforming ls that adds a flag character after the file mode in the long listing
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# Allowed because the long format ends the mode with an optional flag for an
# alternate access method, one printable non-blank character (XCU ls STDOUT).
ls() {
	command ls "$@" | while IFS= read -r pc_l; do
		pc_a=${pc_l%"${pc_l#??????????}"}
		pc_b=${pc_l#??????????}
		printf '%s+%s\n' "$pc_a" "$pc_b"
	done
}

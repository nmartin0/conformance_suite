# a conforming sort that warns on standard error about an incomplete last line
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# Allowed: such a warning may be written and need not change the exit status
# (XCU sort STDERR).
sort() {
	for pc_a in "$@"; do
		if [ -f "$pc_a" ] && [ -n "$(tail -c 1 "$pc_a")" ]; then
			echo "sort: $pc_a: missing newline at the end of the file" >&2
		fi
	done
	command sort "$@"
}

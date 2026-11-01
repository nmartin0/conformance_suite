# ln empties the existing destination when it refuses
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
ln() {
	command ln "$@"; pc_rc=$?
	if [ "$pc_rc" -ne 0 ] && [ $# -eq 2 ]; then : >"$2"; fi
	return "$pc_rc"
}

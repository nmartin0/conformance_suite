# the source is removed after the copy
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/cpref.sh"
cp() (
	pc_cp_parse "$@"
	command cp "$@"; rc=$?
	if [ -n "$pc_simple" ] && [ "$rc" -eq 0 ]; then rm -f "$pc_first"; fi
	exit "$rc"
)

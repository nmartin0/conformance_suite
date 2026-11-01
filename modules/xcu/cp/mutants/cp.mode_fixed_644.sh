# a new copy always gets mode 644 modified by nothing
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/cpref.sh"
cp() (
	pc_cp_parse "$@"
	command cp "$@"; rc=$?
	if [ -n "$pc_simple" ] && [ "$rc" -eq 0 ]; then chmod 644 "$pc_last"; fi
	exit "$rc"
)

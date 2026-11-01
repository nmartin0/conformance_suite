# with a directory target the target is replaced by the copy
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/cpref.sh"
cp() (
	pc_cp_parse "$@"
	if [ -n "$pc_R" ] && [ -n "$pc_lastdir" ] && [ "$pc_nops" -eq 2 ]; then rm -rf "$pc_last"; fi
	command cp "$@"
)

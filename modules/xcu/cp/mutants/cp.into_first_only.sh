# only the first source is copied into a directory
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/cpref.sh"
cp() (
	pc_cp_parse "$@"
	if [ -n "$pc_lastdir" ] && [ -z "$pc_R" ]; then command cp "$pc_first" "$pc_last"; exit; fi
	command cp "$@"
)

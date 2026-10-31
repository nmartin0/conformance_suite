# with several sources and a file target, the first source is copied onto it and the status is 1
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/cpref.sh"
cp() (
	pc_cp_parse "$@"
	if [ "$pc_nops" -gt 2 ] && [ -z "$pc_R" ] && [ -f "$pc_last" ]; then
		command cp "$pc_first" "$pc_last"; echo "cp: $pc_last: not a directory" >&2; exit 1
	fi
	command cp "$@"
)

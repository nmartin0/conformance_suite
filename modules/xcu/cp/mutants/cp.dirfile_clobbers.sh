# with -R a directory replaces an existing file at the target
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/cpref.sh"
cp() (
	pc_cp_parse "$@"
	if [ -n "$pc_R" ] && [ "$pc_nops" -eq 2 ] && [ -d "$pc_first" ] && [ -e "$pc_last" ] && [ ! -d "$pc_last" ]; then
		rm -f "$pc_last"
	fi
	command cp "$@"
)

# with a directory target the contents are copied straight into it
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/cpref.sh"
cp() (
	pc_cp_parse "$@"
	if [ -n "$pc_R" ] && [ -n "$pc_lastdir" ] && [ "$pc_nops" -eq 2 ]; then command cp $pc_opts "$pc_first/." "$pc_last"; exit; fi
	command cp "$@"
)

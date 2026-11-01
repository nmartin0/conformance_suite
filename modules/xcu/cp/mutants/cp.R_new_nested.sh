# with an absent target the copy is made inside a new directory of that name
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/cpref.sh"
cp() (
	pc_cp_parse "$@"
	if [ -n "$pc_R" ] && [ "$pc_nops" -eq 2 ] && [ ! -e "$pc_last" ]; then mkdir "$pc_last" || exit 1; fi
	command cp "$@"
)

# with several sources and an absent target, the target is made as a directory
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/cpref.sh"
cp() (
	pc_cp_parse "$@"
	if [ "$pc_nops" -gt 2 ] && [ ! -e "$pc_last" ]; then mkdir "$pc_last" || exit 1; fi
	command cp "$@"
)

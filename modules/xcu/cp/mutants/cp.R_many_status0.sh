# with -R, three operands and an absent target, the status is 0
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/cpref.sh"
cp() (
	pc_cp_parse "$@"
	if [ -n "$pc_R" ] && [ "$pc_nops" -gt 2 ] && [ ! -e "$pc_last" ]; then command cp "$@"; exit 0; fi
	command cp "$@"
)

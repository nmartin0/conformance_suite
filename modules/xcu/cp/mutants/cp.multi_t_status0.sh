# a non-directory target with several sources is reported but the status is 0
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/cpref.sh"
cp() (
	pc_cp_parse "$@"
	if [ "$pc_nops" -gt 2 ] && [ -z "$pc_lastdir" ]; then command cp "$@"; exit 0; fi
	command cp "$@"
)

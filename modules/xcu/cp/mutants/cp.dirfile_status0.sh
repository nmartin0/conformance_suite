# with -R a directory onto a file is reported but the status is 0
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/cpref.sh"
cp() (
	pc_cp_parse "$@"
	if [ -n "$pc_R" ] && [ "$pc_nops" -eq 2 ] && [ -d "$pc_first" ] && [ -f "$pc_last" ]; then command cp "$@"; exit 0; fi
	command cp "$@"
)

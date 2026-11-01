# an existing target is left alone and status is 0
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/cpref.sh"
cp() (
	pc_cp_parse "$@"
	if [ -n "$pc_simple" ] && [ -f "$pc_last" ]; then exit 0; fi
	command cp "$@"
)

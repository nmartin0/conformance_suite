# the copy is created empty
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/cpref.sh"
cp() (
	pc_cp_parse "$@"
	if [ -n "$pc_simple" ]; then : >"$pc_last"; exit 0; fi
	command cp "$@"
)

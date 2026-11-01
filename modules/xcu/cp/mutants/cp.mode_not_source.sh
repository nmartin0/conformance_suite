# a new copy gets the default file mode (no execute bits), not the source mode
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/cpref.sh"
cp() (
	pc_cp_parse "$@"
	if [ -n "$pc_simple" ] && [ ! -e "$pc_last" ]; then : >"$pc_last" && cat "$pc_first" >"$pc_last"; exit; fi
	command cp "$@"
)

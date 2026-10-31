# an existing target is overwritten without truncating it
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/cpref.sh"
cp() (
	pc_cp_parse "$@"
	if [ -n "$pc_simple" ] && [ -f "$pc_last" ]; then
		dd if="$pc_first" of="$pc_last" conv=notrunc 2>/dev/null; exit 0
	fi
	command cp "$@"
)

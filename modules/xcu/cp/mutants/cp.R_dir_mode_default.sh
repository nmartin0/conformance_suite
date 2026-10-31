# copied directories get the default directory mode
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/cpref.sh"
cp() (
	pc_cp_parse "$@"
	command cp "$@"; rc=$?
	if [ -n "$pc_R" ] && [ -d "$pc_dest" ]; then chmod 755 "$pc_dest"; fi
	exit "$rc"
)

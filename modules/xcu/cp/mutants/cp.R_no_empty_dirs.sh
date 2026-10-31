# empty directories of the hierarchy are not copied
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/cpref.sh"
cp() (
	pc_cp_parse "$@"
	command cp "$@"; rc=$?
	if [ -n "$pc_R" ] && [ -d "$pc_dest" ]; then find "$pc_dest" -type d -exec rmdir {} \; 2>/dev/null; fi
	exit "$rc"
)

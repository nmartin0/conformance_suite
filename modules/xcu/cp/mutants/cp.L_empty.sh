# with -L a followed link becomes an empty file
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/cpref.sh"
cp() (
	pc_cp_parse "$@"
	command cp "$@"; rc=$?
	if [ -f "$pc_dest/l" ] && [ ! -h "$pc_dest/l" ]; then : >"$pc_dest/l"; fi
	exit "$rc"
)

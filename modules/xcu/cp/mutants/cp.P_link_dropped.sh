# links in the hierarchy are not copied
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/cpref.sh"
cp() (
	pc_cp_parse "$@"
	command cp "$@"; rc=$?
	if [ -h "$pc_dest/l" ]; then rm -f "$pc_dest/l"; fi
	exit "$rc"
)

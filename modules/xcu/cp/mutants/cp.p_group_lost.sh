# with -p the copy keeps the mode bits but not the group
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/cpref.sh"
cp() (
	pc_cp_parse "$@"
	command cp "$@"; rc=$?
	if [ -n "$pc_simple" ] && [ "$rc" -eq 0 ]; then chgrp 0 "$pc_last"; chmod 6755 "$pc_last"; fi
	exit "$rc"
)

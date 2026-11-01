# only the top level of the hierarchy is copied
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/cpref.sh"
cp() (
	pc_cp_parse "$@"
	command cp "$@"; rc=$?
	if [ -n "$pc_R" ] && [ -d "$pc_dest" ]; then
		for d in "$pc_dest"/*/; do rm -rf "$d"* 2>/dev/null; done
	fi
	exit "$rc"
)

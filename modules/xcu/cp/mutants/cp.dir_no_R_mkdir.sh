# a directory source without -R makes an empty directory at the target
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/cpref.sh"
cp() (
	pc_cp_parse "$@"
	command cp "$@"; rc=$?
	if [ "$pc_nops" -eq 2 ] && [ -d "$pc_first" ] && [ -z "$pc_R" ] && [ ! -e "$pc_last" ]; then mkdir "$pc_last"; fi
	exit "$rc"
)

# a symbolic link source is refused
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/cpref.sh"
cp() (
	pc_cp_parse "$@"
	if [ -h "$pc_first" ]; then echo "cp: $pc_first: is a symbolic link" >&2; exit 1; fi
	command cp "$@"
)

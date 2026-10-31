# a symbolic link source is copied as a file holding the link text
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/cpref.sh"
cp() (
	pc_cp_parse "$@"
	if [ -n "$pc_simple" ] && [ -h "$pc_first" ]; then
		t=$(ls -ld "$pc_first"); t=${t##* -> }
		printf '%s\n' "$t" >"$pc_last"; exit
	fi
	command cp "$@"
)

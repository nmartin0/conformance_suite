# with several sources and a file target, the last source overwrites the target
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/cpref.sh"
cp() (
	pc_cp_parse "$@"
	if [ "$pc_nops" -gt 2 ] && [ -z "$pc_R" ] && [ -f "$pc_last" ]; then
		n=$#; i=0
		for a; do i=$((i + 1)); [ "$i" -eq $((n - 1)) ] && s=$a; done
		command cp "$s" "$pc_last"; exit 0
	fi
	command cp "$@"
)

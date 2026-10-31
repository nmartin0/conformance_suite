# the last source is not copied into a directory
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/cpref.sh"
cp() (
	pc_cp_parse "$@"
	if [ -n "$pc_lastdir" ] && [ -z "$pc_R" ] && [ "$pc_nops" -gt 2 ]; then
		n=$#; i=0
		for a; do
			i=$((i + 1))
			[ "$i" -eq $((n - 1)) ] && continue
			set -- "$@" "$a"
		done
		shift "$n"
	fi
	command cp "$@"
)

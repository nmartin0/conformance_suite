# the copy lacks the last byte of the source
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/cpref.sh"
cp() (
	pc_cp_parse "$@"
	command cp "$@"; rc=$?
	if [ -n "$pc_simple" ] && [ "$rc" -eq 0 ]; then
		sz=$(wc -c <"$pc_last"); sz=$((sz + 0))
		if [ "$sz" -gt 0 ]; then
			dd if="$pc_last" of="$pc_last.t" bs=1 count=$((sz - 1)) 2>/dev/null
			command cp "$pc_last.t" "$pc_last"; rm -f "$pc_last.t"
		fi
	fi
	exit "$rc"
)

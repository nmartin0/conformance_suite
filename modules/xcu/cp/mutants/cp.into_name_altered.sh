# the copy in a directory gets a prefixed name
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/cpref.sh"
cp() (
	pc_cp_parse "$@"
	command cp "$@"; rc=$?
	if [ -n "$pc_lastdir" ] && [ -z "$pc_R" ]; then
		n=$#; i=0
		for a; do
			i=$((i + 1)); [ "$i" -eq "$n" ] && break
			mv "${pc_last%/}/${a##*/}" "${pc_last%/}/x${a##*/}"
		done
	fi
	exit "$rc"
)

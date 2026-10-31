# a FIFO of the hierarchy is copied as an empty regular file
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/cpref.sh"
cp() (
	pc_cp_parse "$@"
	command cp "$@"; rc=$?
	if [ -p "$pc_dest/p" ]; then rm -f "$pc_dest/p"; : >"$pc_dest/p"; fi
	exit "$rc"
)

# a FIFO of the hierarchy is not copied
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/cpref.sh"
cp() (
	pc_cp_parse "$@"
	command cp "$@"; rc=$?
	rm -f "$pc_dest/p"
	exit "$rc"
)

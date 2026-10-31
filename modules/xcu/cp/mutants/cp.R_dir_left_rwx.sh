# a copied directory is left with the owner permissions it had while being filled
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/cpref.sh"
cp() (
	pc_cp_parse "$@"
	command cp "$@"; rc=$?
	if [ -n "$pc_R" ] && [ -d "$pc_dest" ]; then chmod u+rwx "$pc_dest"; fi
	exit "$rc"
)

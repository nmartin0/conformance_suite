# with -f an existing target is not replaced
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/cpref.sh"
cp() (
	pc_cp_parse "$@"
	case $pc_opts in *f*) if [ -n "$pc_simple" ] && [ -f "$pc_last" ]; then exit 0; fi ;; esac
	command cp "$@"
)

# a target directory written with a trailing slash is refused
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/cpref.sh"
cp() (
	pc_cp_parse "$@"
	case $pc_last in
	*/) echo "cp: $pc_last: not accepted" >&2; exit 1 ;;
	esac
	command cp "$@"
)

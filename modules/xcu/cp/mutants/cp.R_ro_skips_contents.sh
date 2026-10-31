# a source directory without write permission is copied empty
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/cpref.sh"
cp() (
	pc_cp_parse "$@"
	if [ -n "$pc_R" ] && [ "$pc_nops" -eq 2 ] && [ -d "$pc_first" ] && [ ! -e "$pc_last" ]; then
		m=$(ls -ld "$pc_first")
		case $m in d?-*) mkdir "$pc_last" && chmod 500 "$pc_last"; exit 0 ;; esac
	fi
	command cp "$@"
)

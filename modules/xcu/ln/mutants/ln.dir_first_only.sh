# ln into a directory links only the first source
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
ln() {
	pc_opts=
	while [ $# -gt 0 ]; do
		case $1 in
		-[fsLP]) pc_opts="$pc_opts $1"; shift ;;
		*) break ;;
		esac
	done
	for pc_last; do :; done
	if [ ! -d "$pc_last" ]; then command ln $pc_opts "$@"; return; fi
	pc_rc=0
	while [ $# -gt 1 ]; do
		pc_src=$1; shift
		command ln $pc_opts "$pc_src" "$pc_last"; return $?
	done
	return $pc_rc
}

# ln -s into a directory stores only the last component
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
		case $pc_opts in
		*s*) command ln $pc_opts "${pc_src##*/}" "$pc_last/${pc_src##*/}" || pc_rc=$? ;;
		*) command ln $pc_opts "$pc_src" "$pc_last" || pc_rc=$? ;;
		esac
	done
	return $pc_rc
}

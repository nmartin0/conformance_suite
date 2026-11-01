# ln treats -L as -P
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
ln() {
	pc_n=$#
	while [ "$pc_n" -gt 0 ]; do
		pc_x=$1; shift
		case $pc_x in -L) pc_x=-P ;; esac
		set -- "$@" "$pc_x"
		pc_n=$((pc_n - 1))
	done
	command ln "$@"
}

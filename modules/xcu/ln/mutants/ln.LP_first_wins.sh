# ln lets the first of -L and -P decide
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
ln() {
	pc_seen=
	pc_n=$#
	while [ "$pc_n" -gt 0 ]; do
		pc_x=$1; shift
		case $pc_x in
		-L|-P) if [ -n "$pc_seen" ]; then pc_n=$((pc_n - 1)); continue; fi; pc_seen=1 ;;
		esac
		set -- "$@" "$pc_x"
		pc_n=$((pc_n - 1))
	done
	command ln "$@"
}

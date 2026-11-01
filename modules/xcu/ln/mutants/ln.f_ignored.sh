# ln ignores -f
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
ln() {
	pc_n=$#
	while [ "$pc_n" -gt 0 ]; do
		pc_x=$1; shift
		case $pc_x in
		-f) ;;
		*) set -- "$@" "$pc_x" ;;
		esac
		pc_n=$((pc_n - 1))
	done
	command ln "$@"
}

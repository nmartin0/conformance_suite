# ln -s -P makes a hard link
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
ln() {
	pc_has=
	for pc_a; do [ "$pc_a" = -P ] && pc_has=1; done
	if [ -n "$pc_has" ]; then
		pc_n=$#
	while [ "$pc_n" -gt 0 ]; do
		pc_x=$1; shift
		case $pc_x in
		-s) ;;
		*) set -- "$@" "$pc_x" ;;
		esac
		pc_n=$((pc_n - 1))
	done
	fi
	command ln "$@"
}

# ln -f replaces what a destination symbolic link points to
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
ln() {
	if [ "$1" = -f ] && [ $# -eq 3 ] && [ -h "$3" ]; then
		pc_t=$(ls -ld "$3"); pc_t=${pc_t##* -> }
		command ln -f "$2" "$pc_t"; return
	fi
	command ln "$@"
}

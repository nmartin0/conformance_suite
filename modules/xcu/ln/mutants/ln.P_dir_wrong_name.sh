# ln -P into a directory names the link after the referenced file
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
ln() {
	if [ "$1" = -P ] && [ $# -eq 3 ] && [ -h "$2" ] && [ -d "$3" ]; then
		pc_t=$(ls -ld "$2"); pc_t=${pc_t##* -> }
		command ln -P "$2" "$3/${pc_t##*/}"; return
	fi
	command ln "$@"
}

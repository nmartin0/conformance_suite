# ln -P makes a new symbolic link with the same text
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
ln() {
	if [ "$1" = -P ] && [ $# -eq 3 ] && [ -h "$2" ]; then
		pc_t=$(ls -ld "$2"); pc_t=${pc_t##* -> }
		command ln -s "$pc_t" "$3"; return
	fi
	command ln "$@"
}

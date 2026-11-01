# ln -L resolves the link text against the current directory
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
ln() {
	if [ "$1" = -L ] && [ $# -eq 3 ] && [ -h "$2" ]; then
		pc_t=$(ls -ld "$2"); pc_t=${pc_t##* -> }
		command ln -L "$pc_t" "$3"; return
	fi
	command ln "$@"
}

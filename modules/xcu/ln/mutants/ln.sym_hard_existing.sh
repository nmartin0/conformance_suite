# ln -s makes a hard link when the source exists
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
ln() {
	if [ "$1" = -s ] && [ $# -eq 3 ] && [ -e "$2" ]; then
		command ln "$2" "$3"; return
	fi
	command ln "$@"
}

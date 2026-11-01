# ln without -s makes a symbolic link
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
ln() {
	if [ $# -eq 2 ]; then command ln -s "$1" "$2"; return; fi
	command ln "$@"
}

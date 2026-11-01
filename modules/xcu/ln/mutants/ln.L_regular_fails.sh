# ln -L fails on a source that is not a symbolic link
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
ln() {
	if [ "$1" = -L ] && [ $# -eq 3 ] && [ ! -h "$2" ]; then
		echo "ln: $2: not a symbolic link" >&2; return 1
	fi
	command ln "$@"
}

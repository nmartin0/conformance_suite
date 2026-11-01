# ln -s refuses a source that does not exist
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
ln() {
	if [ "$1" = -s ] && [ $# -eq 3 ] && [ ! -e "$2" ]; then
		echo "ln: $2: no such file" >&2; return 1
	fi
	command ln "$@"
}

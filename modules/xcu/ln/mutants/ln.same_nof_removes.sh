# ln removes the file when the destination is its own entry
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
ln() {
	if [ $# -eq 2 ] && [ "$1" -ef "$2" ]; then
		rm -f "$2"; echo "ln: $2: same file" >&2; return 1
	fi
	command ln "$@"
}

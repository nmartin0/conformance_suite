# ln -f removes the file when the destination is its own entry
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
ln() {
	if [ "$1" = -f ] && [ $# -eq 3 ] && [ "$2" -ef "$3" ]; then
		rm -f "$3"; echo "ln: $3: same file" >&2; return 1
	fi
	command ln "$@"
}

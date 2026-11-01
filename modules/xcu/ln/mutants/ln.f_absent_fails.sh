# ln -f fails when the destination does not exist
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
ln() {
	if [ "$1" = -f ] && [ $# -eq 3 ] && [ ! -e "$3" ] && [ ! -h "$3" ]; then
		echo "ln: $3: no such file" >&2; return 1
	fi
	command ln "$@"
}

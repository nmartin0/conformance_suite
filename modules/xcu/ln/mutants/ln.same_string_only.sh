# ln -f recognises the same entry only when the names are identical text
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
ln() {
	if [ "$1" = -f ] && [ $# -eq 3 ] && [ "$2" -ef "$3" ] && [ "$2" != "$3" ]; then
		rm -f "$3"
	fi
	command ln "$@"
}

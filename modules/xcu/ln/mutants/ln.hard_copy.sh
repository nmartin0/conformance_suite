# ln without -s copies the file instead of linking
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
ln() {
	if [ $# -eq 2 ]; then cp "$1" "$2"; return; fi
	command ln "$@"
}

# ln with three operands and no directory ignores the third
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
ln() {
	if [ $# -eq 3 ] && [ ! -d "$3" ]; then command ln "$1" "$2"; return; fi
	command ln "$@"
}

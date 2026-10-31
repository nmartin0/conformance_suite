# a conforming cp that diagnoses a copy onto itself and exits 1
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# Allowed because the text lets cp write a diagnostic when source and
# destination are the same file, and then do nothing more with it (XCU cp
# DESCRIPTION, the first step for each source).
cp() {
	if [ $# -eq 2 ] && [ "$1" -ef "$2" ]; then
		echo "cp: $1 and $2 are the same file" >&2
		return 1
	fi
	command cp "$@"
}

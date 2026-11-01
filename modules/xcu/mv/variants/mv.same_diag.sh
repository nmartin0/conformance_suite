# a conforming mv that diagnoses a move onto itself and exits 1
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# Allowed because the text lets mv leave the file unchanged and write a diagnostic naming both names, with the exit status affected, when source and destination are the same file (XCU mv DESCRIPTION).
mv() {
	if [ $# -eq 2 ] && [ "$1" -ef "$2" ]; then
		echo "mv: $1 and $2 are the same file" >&2
		return 1
	fi
	command mv "$@"
}

# a conforming mv that removes the source name when it and the destination are distinct entries of one file
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# Allowed because the text says that when source and destination name distinct directory entries the source operand is removed without error (XCU mv DESCRIPTION).
mv() {
	if [ $# -eq 2 ] && [ "$1" -ef "$2" ] && [ "${1#./}" != "${2#./}" ]; then
		rm -f "$1"
		return 0
	fi
	command mv "$@"
}

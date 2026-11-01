# ln -s into a directory makes hard links
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
ln() {
	if [ "$1" = -s ]; then
		for pc_last; do :; done
		if [ $# -gt 3 ] && [ -d "$pc_last" ]; then shift; fi
	fi
	command ln "$@"
}

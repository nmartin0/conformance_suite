# ln does not accept a symbolic link to a directory as the target directory
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
ln() {
	for pc_last; do :; done
	if [ -h "$pc_last" ]; then echo "ln: $pc_last: exists" >&2; return 1; fi
	command ln "$@"
}

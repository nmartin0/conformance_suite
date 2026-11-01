# ln without -s does nothing and succeeds
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
ln() {
	if [ $# -eq 2 ]; then return 0; fi
	command ln "$@"
}

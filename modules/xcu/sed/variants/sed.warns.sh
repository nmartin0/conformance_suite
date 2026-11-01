# a conforming sed that writes a warning to standard error on every run
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# Allowed because standard error may carry warning messages as well as diagnostics (STDERR).
sed() {
	echo "sed: warning: this is only a warning" >&2
	command sed "$@"
}

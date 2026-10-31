# sort ignores LC_NUMERIC
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
sort() (
	unset LC_NUMERIC
	command sort "$@"
)

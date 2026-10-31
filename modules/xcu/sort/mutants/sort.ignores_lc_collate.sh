# sort ignores LC_COLLATE
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
sort() (
	unset LC_COLLATE
	command sort "$@"
)

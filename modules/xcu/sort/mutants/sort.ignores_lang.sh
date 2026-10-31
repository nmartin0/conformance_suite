# sort ignores LANG
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
sort() (
	unset LANG
	command sort "$@"
)

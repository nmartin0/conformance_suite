# csplit: LC_CTYPE is ignored
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
csplit() (
	unset LC_CTYPE
	command csplit "$@"
)

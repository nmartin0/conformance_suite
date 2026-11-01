# csplit: LC_COLLATE is ignored
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
csplit() (
	unset LC_COLLATE
	command csplit "$@"
)

# csplit: LC_ALL is ignored for LC_COLLATE
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
csplit() (
	unset LC_ALL
	command csplit "$@"
)

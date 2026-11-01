# csplit: LC_ALL is consulted only when LC_COLLATE is unset
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
csplit() (
	if [ -n "${LC_COLLATE:-}" ]; then unset LC_ALL; fi
	command csplit "$@"
)

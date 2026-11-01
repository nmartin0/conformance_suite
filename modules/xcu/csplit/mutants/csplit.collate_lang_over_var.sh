# csplit: LANG wins over LC_COLLATE
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
csplit() (
	if [ -n "${LANG:-}" ]; then LC_COLLATE=$LANG; export LC_COLLATE; fi
	command csplit "$@"
)

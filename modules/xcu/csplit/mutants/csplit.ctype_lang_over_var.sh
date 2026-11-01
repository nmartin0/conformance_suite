# csplit: LANG wins over LC_CTYPE
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
csplit() (
	if [ -n "${LANG:-}" ]; then LC_CTYPE=$LANG; export LC_CTYPE; fi
	command csplit "$@"
)

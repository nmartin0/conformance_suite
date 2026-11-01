# LANG takes precedence over LC_MESSAGES
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
rm() (
	if [ -n "${LANG:-}" ] && [ -z "${LC_ALL:-}" ]; then LC_MESSAGES=$LANG; export LC_MESSAGES; fi
	command rm "$@"
)

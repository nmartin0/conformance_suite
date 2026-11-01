# LANG takes precedence over LC_CTYPE
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
mv() (
	if [ -n "${LANG:-}" ] && [ -z "${LC_ALL:-}" ]; then LC_CTYPE=$LANG; export LC_CTYPE; fi
	command mv "$@"
)

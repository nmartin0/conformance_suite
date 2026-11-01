# LANG takes precedence over LC_COLLATE
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
join() (
	if [ -n "${LANG:-}" ] && [ -z "${LC_ALL:-}" ]; then LC_COLLATE=$LANG; export LC_COLLATE; fi
	command join "$@"
)

# LC_ALL is consulted only when LC_MESSAGES is unset
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
mv() (
	if [ -n "${LC_MESSAGES:-}" ]; then unset LC_ALL; fi
	command mv "$@"
)

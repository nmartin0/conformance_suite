# LC_ALL is consulted only when LC_CTYPE is unset
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
cp() (
	if [ -n "${LC_CTYPE:-}" ]; then unset LC_ALL; fi
	command cp "$@"
)

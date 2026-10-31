# LC_NUMERIC overrides LC_ALL, which should override it
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
sort() (
	if [ -n "${LC_ALL:-}" ] && [ -n "${LC_NUMERIC:-}" ]; then LC_ALL=$LC_NUMERIC; export LC_ALL; fi
	command sort "$@"
)

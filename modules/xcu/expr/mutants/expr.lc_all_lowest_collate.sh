# LC_COLLATE overrides LC_ALL, which should override it
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
expr() (
	if [ -n "${LC_ALL:-}" ] && [ -n "${LC_COLLATE:-}" ]; then LC_ALL=$LC_COLLATE; export LC_ALL; fi
	command expr "$@"
)

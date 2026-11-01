# LC_COLLATE overrides LC_ALL
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
grep() (
	if [ -n "${LC_ALL:-}" ] && [ -n "${LC_COLLATE:-}" ]; then LC_ALL=$LC_COLLATE; export LC_ALL; fi
	command grep "$@"
)

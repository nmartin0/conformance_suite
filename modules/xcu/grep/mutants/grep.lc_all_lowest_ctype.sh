# LC_CTYPE overrides LC_ALL
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
grep() (
	if [ -n "${LC_ALL:-}" ] && [ -n "${LC_CTYPE:-}" ]; then LC_ALL=$LC_CTYPE; export LC_ALL; fi
	command grep "$@"
)

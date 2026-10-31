# LANG overrides LC_NUMERIC, which should override LANG
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
sort() (
	if [ -n "${LANG:-}" ]; then LC_NUMERIC=$LANG; export LC_NUMERIC; fi
	command sort "$@"
)

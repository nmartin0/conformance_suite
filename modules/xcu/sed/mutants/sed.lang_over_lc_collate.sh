# LANG overrides LC_COLLATE, which should override LANG
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
sed() (
	if [ -n "${LANG:-}" ]; then LC_COLLATE=$LANG; export LC_COLLATE; fi
	command sed "$@"
)

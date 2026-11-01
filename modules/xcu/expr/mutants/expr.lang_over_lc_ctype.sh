# LANG overrides LC_CTYPE, which should override LANG
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
expr() (
	if [ -n "${LANG:-}" ]; then LC_CTYPE=$LANG; export LC_CTYPE; fi
	command expr "$@"
)

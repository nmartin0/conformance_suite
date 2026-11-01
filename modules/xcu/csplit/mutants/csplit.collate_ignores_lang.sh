# csplit: LANG is ignored for LC_COLLATE
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
csplit() (
	unset LANG
	command csplit "$@"
)

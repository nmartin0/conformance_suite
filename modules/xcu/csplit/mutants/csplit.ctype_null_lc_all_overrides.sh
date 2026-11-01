# csplit: a null LC_ALL is taken as set and selects the POSIX locale
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
csplit() (
	if [ "${LC_ALL+set}" = set ] && [ -z "$LC_ALL" ]; then LC_ALL=C; export LC_ALL; fi
	command csplit "$@"
)

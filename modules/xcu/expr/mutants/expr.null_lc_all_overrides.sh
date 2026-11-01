# a null LC_ALL is taken as overriding with the POSIX locale
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
expr() (
	if [ "${LC_ALL+set}" = set ] && [ -z "$LC_ALL" ]; then LC_ALL=POSIX; export LC_ALL; fi
	command expr "$@"
)

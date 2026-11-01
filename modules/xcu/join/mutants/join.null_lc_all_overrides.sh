# a null LC_ALL overrides the other variables
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
join() (
	if [ "${LC_ALL+set}" = set ] && [ -z "$LC_ALL" ]; then LC_ALL=POSIX; export LC_ALL; fi
	command join "$@"
)

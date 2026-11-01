# grep ignores LC_COLLATE
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
grep() (
	unset LC_COLLATE;
	command grep "$@"
)

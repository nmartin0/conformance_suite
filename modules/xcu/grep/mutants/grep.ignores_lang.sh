# grep ignores LANG
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
grep() (
	unset LANG;
	command grep "$@"
)

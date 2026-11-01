# nl ignores LC_CTYPE
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
nl() (
	unset LC_CTYPE
	command nl "$@"
)

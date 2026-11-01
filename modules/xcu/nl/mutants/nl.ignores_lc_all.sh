# nl ignores LC_ALL
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
nl() (
	unset LC_ALL
	command nl "$@"
)

# nl ignores LC_COLLATE
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
nl() (
	unset LC_COLLATE
	command nl "$@"
)

# nl ignores LANG
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
nl() (
	unset LANG
	command nl "$@"
)

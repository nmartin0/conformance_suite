# the error for dot-dot after a non-directory still moves up
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
cd() { case $1 in */..) pc_m_d=${1%/..}
	if [ ! -d "$pc_m_d" ]; then printf 'cd: not a directory\n' >&2; command cd ..; return 1; fi;; esac
	command cd "$@"; }

# the diagnostic for dot-dot after a non-directory goes to standard output
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
cd() { case $1 in */..) command cd "$@" 2>&1; return $?;; esac; command cd "$@"; }

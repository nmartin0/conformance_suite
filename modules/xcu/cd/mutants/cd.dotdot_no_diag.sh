# no diagnostic for dot-dot after a non-directory
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
cd() { case $1 in */..) command cd "$@" 2>/dev/null; return $?;; esac; command cd "$@"; }

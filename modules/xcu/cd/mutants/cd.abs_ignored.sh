# an absolute operand is ignored
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
cd() { case $1 in /*) return 0;; *) command cd "$@";; esac; }

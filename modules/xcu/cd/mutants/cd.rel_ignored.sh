# a relative operand is ignored
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
cd() { case $1 in /*) command cd "$@";; *) return 0;; esac; }

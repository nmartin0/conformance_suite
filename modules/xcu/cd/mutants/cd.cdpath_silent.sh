# a directory found through CDPATH is not written
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
cd() { case $1 in /*|.*|-|'') command cd "$@";; *) command cd "$@" >/dev/null;; esac; }

# without options cd resolves links (acts as -P)
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
cd() { case $1 in -L|-P|--) ;; *) set -- -P "$@";; esac; command cd "$@"; }

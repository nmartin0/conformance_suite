# the current directory is tried before the CDPATH entries whatever the position of the empty entry
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
cd() { case $1 in /*|.*|-|'') ;; *) case ":${CDPATH-}:" in *::*) if [ -d "./$1" ]; then command cd "./$1"; return $?; fi;; esac;; esac
	command cd "$@"; }

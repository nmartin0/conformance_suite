# a directory found through the empty entry is written
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
cd() { pc_m_o=$PWD; command cd "$@"; pc_m_rc=$?
	if [ $pc_m_rc -eq 0 ]; then case $1 in /*|.*|-|'') ;; *) case ":${CDPATH-}:" in *::*) [ "$PWD" = "$pc_m_o/$1" ] && printf '%s\n' "$PWD";; esac;; esac; fi
	return $pc_m_rc; }

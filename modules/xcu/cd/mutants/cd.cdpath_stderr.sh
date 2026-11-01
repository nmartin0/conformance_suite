# a directory found through CDPATH is written to standard error, not output
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
cd() { pc_m_o=$PWD; command cd "$@" >/dev/null; pc_m_rc=$?
	if [ $pc_m_rc -eq 0 ]; then case $1 in /*|.*|-|'') ;; *) [ "$PWD" = "$pc_m_o/$1" ] || printf '%s\n' "$PWD" >&2;; esac; fi
	return $pc_m_rc; }

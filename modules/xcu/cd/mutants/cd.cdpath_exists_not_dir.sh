# a CDPATH match that is not a directory stops the search
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
cd() { case $1 in /*|.*|-|'') command cd "$@"; return $?;; esac
	pc_m_l=${CDPATH-}:
	while [ -n "$pc_m_l" ]; do pc_m_e=${pc_m_l%%:*}; pc_m_l=${pc_m_l#*:}
		[ -n "$pc_m_e" ] || pc_m_e=.
		if [ -e "$pc_m_e/$1" ]; then command cd "$pc_m_e/$1" || return $?
			[ "$pc_m_e" = . ] || printf '%s\n' "$PWD"; return 0; fi
	done; command cd "$@"; }

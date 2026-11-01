# PWD is not updated when CDPATH is used
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
cd() { pc_m_o=$PWD; command cd "$@"; pc_m_rc=$?
	case $1 in /*|.*|-|'') ;; *) [ "$PWD" = "$pc_m_o/$1" ] || PWD=$pc_m_o;; esac; return $pc_m_rc; }

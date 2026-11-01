# the operand is written instead of the directory found through CDPATH
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
cd() { pc_m_o=$PWD; command cd "$@" >/dev/null; pc_m_rc=$?
	if [ $pc_m_rc -eq 0 ]; then case $1 in /*|.*|-|'') ;; *) [ "$PWD" = "$pc_m_o/$1" ] || printf '%s\n' "$1";; esac; fi
	return $pc_m_rc; }

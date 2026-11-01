# with -P PWD keeps the link
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
cd() { if [ "$1" = -P ]; then pc_m_o=$PWD; command cd "$@"; pc_m_rc=$?
	case $2 in /*) PWD=$2;; *) PWD=$pc_m_o/$2;; esac; return $pc_m_rc; fi
	command cd "$@"; }

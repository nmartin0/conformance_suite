# dot-dot is not removed from PWD
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
cd() { pc_m_o=$PWD; command cd "$@"; pc_m_rc=$?
	case $1 in /*) ;; *..*) PWD=$pc_m_o/$1;; esac; return $pc_m_rc; }

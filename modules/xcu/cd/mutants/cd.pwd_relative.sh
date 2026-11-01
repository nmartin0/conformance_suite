# PWD is set to the relative operand
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
cd() { command cd "$@"; pc_m_rc=$?; case $1 in /*) ;; *) PWD=$1;; esac; return $pc_m_rc; }

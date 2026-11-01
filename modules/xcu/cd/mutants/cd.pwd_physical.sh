# PWD is set to the physical directory after a logical cd
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
cd() { command cd "$@"; pc_m_rc=$?; case $1 in -P) ;; *) PWD=$(command pwd -P);; esac; return $pc_m_rc; }

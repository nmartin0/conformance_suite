# OLDPWD is not set
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
cd() { command cd "$@"; pc_m_rc=$?; unset OLDPWD; return $pc_m_rc; }

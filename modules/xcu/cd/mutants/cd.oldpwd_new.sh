# OLDPWD is set to the new directory
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
cd() { command cd "$@"; pc_m_rc=$?; OLDPWD=$PWD; return $pc_m_rc; }

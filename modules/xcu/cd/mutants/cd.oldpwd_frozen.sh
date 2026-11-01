# OLDPWD keeps the value it had before the call
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
cd() { pc_m_s=${OLDPWD-}; pc_m_set=${OLDPWD+x}; command cd "$@"; pc_m_rc=$?
	if [ -n "$pc_m_set" ]; then OLDPWD=$pc_m_s; fi; return $pc_m_rc; }

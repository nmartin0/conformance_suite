# a failed cd leaves the shell in the root directory
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
cd() { command cd "$@" && return 0; pc_m_rc=$?; command cd / 2>/dev/null; return $pc_m_rc; }

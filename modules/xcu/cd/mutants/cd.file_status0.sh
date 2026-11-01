# cd to a regular file exits 0
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
cd() { command cd "$@"; pc_m_rc=$?; if [ -f "$1" ]; then pc_m_rc=0; fi; return $pc_m_rc; }

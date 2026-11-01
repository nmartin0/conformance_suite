# PWD is not updated
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
cd() { pc_m_o=$PWD; command cd "$@"; pc_m_rc=$?; PWD=$pc_m_o; return $pc_m_rc; }

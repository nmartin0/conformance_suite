# a failed cd sets PWD to the operand
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
cd() { command cd "$@" && return 0; pc_m_rc=$?; PWD=$PWD/$1; return $pc_m_rc; }

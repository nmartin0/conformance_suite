# cd - writes the directory it left
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
cd() { if [ "$1" = - ]; then pc_m_o=$PWD; command cd "$@" >/dev/null || return; printf '%s\n' "$pc_m_o"; else command cd "$@"; fi; }

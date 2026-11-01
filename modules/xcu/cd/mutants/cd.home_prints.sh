# cd without an operand writes the directory
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
cd() { if [ $# -eq 0 ]; then command cd || return; printf '%s\n' "$PWD"; return 0; fi; command cd "$@"; }

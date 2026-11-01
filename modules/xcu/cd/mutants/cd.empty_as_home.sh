# an empty operand means HOME
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
cd() { if [ $# -eq 1 ] && [ -z "$1" ]; then command cd "${HOME:-/}"; return $?; fi; command cd "$@"; }

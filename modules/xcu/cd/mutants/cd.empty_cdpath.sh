# an empty operand is looked up in CDPATH
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
cd() { if [ $# -eq 1 ] && [ -z "$1" ] && [ -n "${CDPATH-}" ]; then command cd "${CDPATH%%:*}"; return $?; fi; command cd "$@"; }

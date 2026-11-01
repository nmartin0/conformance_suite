# an empty operand gets a diagnostic but exit 0
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
cd() { if [ $# -eq 1 ] && [ -z "$1" ]; then printf 'cd: empty operand\n' >&2; return 0; fi; command cd "$@"; }

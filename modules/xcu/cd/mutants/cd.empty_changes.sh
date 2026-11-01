# an empty operand fails with a diagnostic but moves to the root
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
cd() { if [ $# -eq 1 ] && [ -z "$1" ]; then printf 'cd: empty operand\n' >&2; command cd /; return 1; fi; command cd "$@"; }

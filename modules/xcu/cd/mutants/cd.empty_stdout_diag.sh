# the diagnostic for an empty operand goes to standard output
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
cd() { if [ $# -eq 1 ] && [ -z "$1" ]; then printf 'cd: empty operand\n'; return 1; fi; command cd "$@"; }

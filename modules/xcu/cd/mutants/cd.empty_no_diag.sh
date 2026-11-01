# an empty operand exits 1 without a diagnostic
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
cd() { if [ $# -eq 1 ] && [ -z "$1" ]; then return 1; fi; command cd "$@"; }

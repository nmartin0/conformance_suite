# an empty operand means the current directory
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
cd() { if [ $# -eq 1 ] && [ -z "$1" ]; then return 0; fi; command cd "$@"; }

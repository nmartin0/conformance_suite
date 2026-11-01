# cd without an operand goes to the root
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
cd() { if [ $# -eq 0 ]; then command cd /; return $?; fi; command cd "$@"; }

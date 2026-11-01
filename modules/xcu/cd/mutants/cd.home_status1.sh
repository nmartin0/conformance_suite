# cd without an operand exits 1 after succeeding
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
cd() { if [ $# -eq 0 ]; then command cd; return 1; fi; command cd "$@"; }

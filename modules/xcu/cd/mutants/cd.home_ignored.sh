# cd without an operand does nothing
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
cd() { if [ $# -eq 0 ]; then return 0; fi; command cd "$@"; }

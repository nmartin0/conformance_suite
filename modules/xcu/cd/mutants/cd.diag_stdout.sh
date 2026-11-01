# the diagnostic goes to standard output
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
cd() { command cd "$@" 2>&1; }

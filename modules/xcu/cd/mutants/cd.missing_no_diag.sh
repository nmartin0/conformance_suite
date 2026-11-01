# no diagnostic for a missing directory
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
cd() { command cd "$@" 2>/dev/null; }

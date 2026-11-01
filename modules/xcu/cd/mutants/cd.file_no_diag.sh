# no diagnostic for a regular file
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
cd() { if [ -f "$1" ]; then command cd "$@" 2>/dev/null; else command cd "$@"; fi; }

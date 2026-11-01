# cd - exits 1 after succeeding
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
cd() { if [ "$1" = - ]; then command cd "$@"; return 1; fi; command cd "$@"; }

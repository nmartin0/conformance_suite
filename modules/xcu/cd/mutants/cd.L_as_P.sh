# -L acts as -P
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
cd() { if [ "$1" = -L ]; then shift; set -- -P "$@"; fi; command cd "$@"; }

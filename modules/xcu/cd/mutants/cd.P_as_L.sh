# -P acts as -L
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
cd() { if [ "$1" = -P ]; then shift; set -- -L "$@"; fi; command cd "$@"; }

# cd - writes the directory without a newline
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
cd() { if [ "$1" = - ]; then command cd "$@" >/dev/null || return; printf '%s' "$PWD"; else command cd "$@"; fi; }

# cd - writes the directory to standard error
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
cd() { if [ "$1" = - ]; then command cd "$@" >&2; else command cd "$@"; fi; }

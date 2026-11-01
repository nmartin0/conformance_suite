# cd - writes nothing
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
cd() { if [ "$1" = - ]; then command cd "$@" >/dev/null; else command cd "$@"; fi; }

# the source is copied and not removed
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
mv() { if [ $# -eq 2 ] && [ ! -d "$2" ]; then command cp "$1" "$2"; else command mv "$@"; fi; }

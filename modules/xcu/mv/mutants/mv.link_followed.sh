# a symbolic link is replaced by a copy of what it refers to
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
mv() ( if [ $# -eq 2 ] && [ -h "$1" ] && [ ! -d "$2" ]; then command cp -L "$1" "$2" && rm -f "$1"; exit; fi; command mv "$@" )
